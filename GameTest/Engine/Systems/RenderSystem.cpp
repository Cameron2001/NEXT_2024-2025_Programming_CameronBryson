#include "stdafx.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/HiddenLineRemoval.h"
#include "Engine/Graphics/Renderer.h"
#include "RenderSystem.h"
#include <concurrent_vector.h>
#include <Engine/Core/Components.h>
#include <Engine/Graphics/Edge.h>
#include <Engine/Graphics/Model.h>
#include <Engine/Graphics/Renderer2D.h>
#include <Engine/Graphics/Triangle.h>
#include <Engine/Managers/GraphicsManager.h>
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>
#include <vector>

RenderSystem::RenderSystem(Registry *registry, GraphicsManager *graphicsManager, Camera *camera)
    : m_registry(registry), m_graphicsManager(graphicsManager), m_camera(camera), m_hiddenLineRemoval(), m_triangles(),
      m_triangleList(), m_visibleSegments()
{
    m_triangles.reserve(2000);
    m_triangleList.reserve(2000);
    m_visibleSegments.reserve(10000);
}

void RenderSystem::Init()
{
}
void RenderSystem::Update()
{
    auto viewProjectionMatrix = m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix();
    auto modelView = m_registry->CreateView<TransformComponent, ModelComponent>();

    // Process entities in parallel
    modelView.ParallelForEach([&](const auto &entityTuple) {
        auto &transform = std::get<1>(entityTuple);
        auto &modelComponent = std::get<2>(entityTuple);

        auto &model = m_graphicsManager->GetModel(modelComponent.modelName);
        auto modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) *
                           Matrix4::CreateScaleMatrix(transform.Scale) *
                           Matrix4::CreateEulerAngleMatrixXYZ(transform.Rotation);
        auto mvpMatrix = viewProjectionMatrix * modelMatrix;

        // For each mesh in the model
        for (const auto &mesh : model.meshes)
        {
            // For each triangle in the mesh
            for (const auto &face : mesh.triangles)
            {
                // Transform vertices
                FVector3 mvpVertex0 = mvpMatrix.TransformWithPerspectiveDivide(face.v0);
                FVector3 mvpVertex1 = mvpMatrix.TransformWithPerspectiveDivide(face.v1);
                FVector3 mvpVertex2 = mvpMatrix.TransformWithPerspectiveDivide(face.v2);

                if (Renderer::QuickReject(Edge3D(mvpVertex0, mvpVertex1), Edge3D(mvpVertex1, mvpVertex2),
                                          Edge3D(mvpVertex2, mvpVertex0)))
                {
                    continue;
                }

                // Backface culling
                const float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                                          ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
                if (determinant < 0)
                {
                    continue;
                }

                // Pre-Compute average depth (Z value)
                float avgZ = (mvpVertex0.Z + mvpVertex1.Z + mvpVertex2.Z) / 3.0f;

                // Create 2D vertices
                FVector2 v0(mvpVertex0.X, mvpVertex0.Y);
                FVector2 v1(mvpVertex1.X, mvpVertex1.Y);
                FVector2 v2(mvpVertex2.X, mvpVertex2.Y);

                // Add triangle to the concurrent vector
                m_triangles.push_back(Triangle2D(v0, v1, v2, avgZ));
            }
        }
    });

    m_triangleList.reserve(m_triangles.size());
    m_triangleList.insert(m_triangleList.end(), std::make_move_iterator(m_triangles.begin()),
                          std::make_move_iterator(m_triangles.end()));
    m_visibleSegments = m_hiddenLineRemoval.removeHiddenLines(m_triangleList);
}
void RenderSystem::Render()
{

    for (const auto &edge : m_visibleSegments)
    {
        Renderer2D::DrawLine(edge, {1.0f, 1.0f, 1.0f});
    }
    m_triangles.clear();
    m_triangleList.clear();
    m_visibleSegments.clear();
}

void RenderSystem::LateRender()
{
    //  UI Rendering
}

void RenderSystem::Shutdown()
{
}

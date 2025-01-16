#include "stdafx.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/HiddenLineRemoval.h"
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
#include <cmath>

RenderSystem::RenderSystem(Registry *registry, GraphicsManager *graphicsManager, Camera *camera)
    : m_registry(registry), m_modelView(registry), m_graphicsManager(graphicsManager), m_camera(camera),
      m_hiddenLineRemoval(), m_triangles(), m_triangleList(), m_visibleSegments(), m_textView(registry)
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
    m_modelView.Update();
    auto viewProjectionMatrix = m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix();

    // Process entities in parallel with updated lambda signature
    m_modelView.ParallelForEach([&](Entity entity, TransformComponent &transform, ModelComponent &modelComponent) {
        auto &model = m_graphicsManager->GetModel(modelComponent.modelName);
        auto modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) *
                           Matrix4::CreateScaleMatrix(transform.Scale) * transform.Rotation.GetRotationMatrix4();
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

                if (RejectTriangle(mvpVertex0, mvpVertex1, mvpVertex2))
                {
                    continue;
                }

                // Backface culling
                /*const float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                                          ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
                if (determinant < 0)
                {
                    continue;
                }*/

                // Pre-Compute average depth (Z value)
                float avgZ = (mvpVertex0.z + mvpVertex1.z + mvpVertex2.z) / 3.0f;

                // Create 2D vertices
                FVector2 v0(mvpVertex0.x, mvpVertex0.y);
                FVector2 v1(mvpVertex1.x, mvpVertex1.y);
                FVector2 v2(mvpVertex2.x, mvpVertex2.y);

                // Add triangle to the concurrent vector
                m_triangles.push_back(Triangle2D(v0, v1, v2, avgZ));
            }
        }
    });

    m_triangleList.reserve(m_triangles.size());
    m_triangleList.insert(m_triangleList.end(), std::make_move_iterator(m_triangles.begin()),
                          std::make_move_iterator(m_triangles.end()));
    m_visibleSegments = m_hiddenLineRemoval.RemoveHiddenLines(m_triangleList);
}
void RenderSystem::Render()
{
    for (const auto &edge : m_visibleSegments)
    {
        if (std::isnan(edge.start.x) || std::isnan(edge.start.y) || std::isnan(edge.end.x) || std::isnan(edge.end.y))
        {
            continue; // Skip invalid edges
        }
        Renderer2D::DrawLine(edge, {1.0f, 1.0f, 1.0f});
    }
    m_triangles.clear();
    m_triangleList.clear();
    m_visibleSegments.clear();
}

void RenderSystem::LateRender()
{
    //  UI Rendering
    m_textView.Update();
    m_textView.ForEach([&](Entity entity, TextComponent &text) {
        Renderer2D::PrintText(text.text, text.position, {1.0f, 1.0f, 1.0f});
    });
}

void RenderSystem::Shutdown()
{
}

bool RenderSystem::RejectTriangle(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2)
{
    const float determinant = ((v1.x - v0.x) * (v2.y - v0.y)) - ((v1.y - v0.y) * (v2.x - v0.x));
    if (determinant < 0)
    {
        return true;
    }
    return false;
}

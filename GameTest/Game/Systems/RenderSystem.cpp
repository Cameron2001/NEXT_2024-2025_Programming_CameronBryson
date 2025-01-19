#include "stdafx.h"
#include "Game/Graphics/Camera.h"
#include "Game/Graphics/HiddenLineRemoval.h"
#include "RenderSystem.h"
#include <concurrent_vector.h>
#include <Game/Core/Components.h>
#include <Game/Graphics/Edge.h>
#include <Game/Graphics/Model.h>
#include <Game/Graphics/Renderer.h>
#include <Game/Graphics/Triangle.h>
#include <Game/Managers/GraphicsManager.h>
#include <Game/Math/Matrix4.h>
#include <Game/Math/Vector2.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>
#include <vector>
#include <cmath>

RenderSystem::RenderSystem(Registry *registry, GraphicsManager *graphicsManager, Camera *camera)
    : m_registry(registry), m_modelView(registry), m_textView(registry), m_graphicsManager(graphicsManager),
      m_camera(camera), m_hiddenLineRemoval(), m_triangles(), m_triangleList(), m_visibleSegments()
{
    m_triangles.local().reserve(2000);
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

    m_modelView.ParallelForEach([&](Entity entity, TransformComponent &transform, ModelComponent &modelComponent) {
        if (modelComponent.layer < 0)
        {
            return;
        }
        auto &model = m_graphicsManager->GetModel(modelComponent.modelName);

        auto modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) *
                           transform.Rotation.GetRotationMatrix4() * Matrix4::CreateScaleMatrix(transform.Scale);

        auto mvpMatrix = viewProjectionMatrix*modelMatrix;

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
                float avgZ = (mvpVertex0.z + mvpVertex1.z + mvpVertex2.z) / 3.0f;

                // Create 2D vertices
                FVector2 v0(mvpVertex0.x, mvpVertex0.y);
                FVector2 v1(mvpVertex1.x, mvpVertex1.y);
                FVector2 v2(mvpVertex2.x, mvpVertex2.y);

                // Add triangle to the concurrent vector
                m_triangles.local().emplace_back(v0, v1, v2, avgZ, modelComponent.color, modelComponent.layer);
            }
        }
    });

    m_triangles.combine_each([&](std::vector<Triangle2D> &triangles) {
        m_triangleList.insert(m_triangleList.end(), std::make_move_iterator(triangles.begin()),
                              std::make_move_iterator(triangles.end()));
    });
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
        Renderer::DrawLine(edge, edge.color);
    }
    m_triangles.clear();
    m_triangleList.clear();
    m_visibleSegments.clear();
}

void RenderSystem::LateRender()
{
    //  UI Rendering
    m_textView.Update();
    m_textView.ForEach(
        [&](Entity entity, TextComponent &text) { Renderer::PrintText(text.text, text.position, {1.0f, 1.0f, 1.0f}); });
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

    // For +Z forward, the closest point should be the minimum z value
    auto closestPoint = std::min({v0.z, v1.z, v2.z});

    // Adjust the depth range check for +Z forward
    if (closestPoint < -1.0f)
    {
        return true; // Reject triangles outside the [0, 1] depth range
    }
    return false;
}

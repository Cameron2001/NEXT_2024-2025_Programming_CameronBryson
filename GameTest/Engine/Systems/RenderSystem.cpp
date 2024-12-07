#include "stdafx.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/Renderer.h"
#include "RenderSystem.h"
#include <Engine/Core/Components.h>
#include <Engine/Graphics/Vertex.h>
#include <Engine/Managers/GraphicsManager.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/ComponentArray.h>
#include <vector>

RenderSystem::RenderSystem()
{
}

void RenderSystem::Init(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<Camera> camera,
                        std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray,
                        std::shared_ptr<ComponentArray<ModelComponent>> modelComponentArray,
                        std::shared_ptr<ComponentArray<TextComponent>> textComponentArray)
{
    m_graphicsManager = graphicsManager;
    m_camera = camera;
    m_transformComponentArray = transformComponentArray;
    m_modelComponentArray = modelComponentArray;
    m_textComponentArray = textComponentArray;
    const FVector3 half_extents(1.0f, 1.0f, 1.0f);
    const std::vector<Vertex> cubeVertices = {
        {-half_extents.X, -half_extents.Y, -half_extents.Z},
        {half_extents.X, -half_extents.Y, -half_extents.Z},
        {half_extents.X, half_extents.Y, -half_extents.Z},
        {-half_extents.X, half_extents.Y, -half_extents.Z},
        {-half_extents.X, -half_extents.Y, half_extents.Z},
        {half_extents.X, -half_extents.Y, half_extents.Z},
        {half_extents.X, half_extents.Y, half_extents.Z},
        {-half_extents.X, half_extents.Y, half_extents.Z}
    };

    // Define cube faces using quad_indices (triplets of indices)
    const std::vector<Face> cubeFaces = {
        {0, 1, 3}, {3, 1, 2},
        {1, 5, 2}, {2, 5, 6},
        {5, 4, 6}, {6, 4, 7},
        {4, 0, 7}, {7, 0, 3},
        {3, 2, 7}, {7, 2, 6},
        {4, 5, 0}, {0, 5, 1}
    };

    // Create the cube mesh using vertices and faces
    Mesh cubeMesh(cubeVertices, cubeFaces);

    // Create the cube model containing the mesh
    Model cubeModel({cubeMesh});

    // Add the cube model to the graphics manager with the name "Cube"
    graphicsManager->AddModel("Cube", cubeModel);
}

void RenderSystem::Render()
{
    auto graphicsManager = m_graphicsManager.lock();
    auto camera = m_camera.lock();
    auto transformComponentArray = m_transformComponentArray.lock();
    auto modelComponentArray = m_modelComponentArray.lock();
    auto textComponentArray = m_textComponentArray.lock();

    if (!graphicsManager || !camera || !transformComponentArray || !modelComponentArray || !textComponentArray)
    {
        return;
    }
    auto viewProjectionMatrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();
    auto transformModelIDs = transformComponentArray->GetEntityIntersection(modelComponentArray->GetEntities());
    for (auto ID : transformModelIDs)
    {
        auto &transform = transformComponentArray->GetComponent(ID);
        auto &model = modelComponentArray->GetComponent(ID);
        auto &modelData = graphicsManager->GetModel(model.modelName);
        auto modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) *
                           Matrix4::CreateScaleMatrix(transform.Scale) *
                           Matrix4::CreateEulerAngleMatrixXYZ(transform.Rotation);
        auto mvpMatrix = viewProjectionMatrix * modelMatrix;
        Renderer::RenderModel(modelData, mvpMatrix);
    }
}

void RenderSystem::Shutdown()
{
}
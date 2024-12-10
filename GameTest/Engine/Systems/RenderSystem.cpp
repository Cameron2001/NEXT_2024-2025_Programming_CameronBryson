#include "stdafx.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/Renderer.h"
#include "RenderSystem.h"
#include <Engine/Core/Components.h>
#include <Engine/Managers/GraphicsManager.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/ComponentArray.h>
#include <vector>

RenderSystem::RenderSystem(Registry* registry, GraphicsManager* graphicsManager, Camera* camera)
{
    m_registry = registry;
    m_graphicsManager = graphicsManager;
    m_camera = camera;
}

void RenderSystem::Init()
{
}

void RenderSystem::Render()
{
    auto viewProjectionMatrix = m_camera->GetProjectionMatrix() * m_camera->GetViewMatrix();

    auto view = m_registry->CreateView<TransformComponent, ModelComponent>();

    for (auto it = view.begin(); it != view.end(); ++it)
    {
        auto &transform = std::get<0>(*it);
        auto &model = std::get<1>(*it);

        auto &modelData = m_graphicsManager->GetModel(model.modelName);

        auto modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) *
                           Matrix4::CreateScaleMatrix(transform.Scale) *
                           Matrix4::CreateEulerAngleMatrixXYZ(transform.Rotation);
        auto normalMatrix = modelMatrix.Inverse().Transpose();
        auto mvpMatrix = viewProjectionMatrix * modelMatrix;

        Renderer::QueueModel(modelData, mvpMatrix);
    }
    Renderer::SubmitQueue();
}

void RenderSystem::Shutdown()
{
}
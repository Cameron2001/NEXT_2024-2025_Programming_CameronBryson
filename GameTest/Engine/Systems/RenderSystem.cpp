#include "stdafx.h"
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/Renderer.h"
#include "RenderSystem.h"
#include <concurrent_vector.h>
#include <Engine/Core/Components.h>
#include <Engine/Graphics/Model.h>
#include <Engine/Managers/GraphicsManager.h>
#include <Engine/Math/Matrix4.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>
#include <utility>

RenderSystem::RenderSystem(Registry *registry, GraphicsManager *graphicsManager, Camera *camera)
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
    auto modelView = m_registry->CreateView<TransformComponent, ModelComponent>();

    // Thread-safe container for queued models
    concurrency::concurrent_vector<std::pair<Model, Matrix4>> queuedModels;

    // Use the new parallel forEach
    modelView.ParallelForEach([&](const auto &entityTuple) {
        auto &transform = std::get<1>(entityTuple);
        auto &model = std::get<2>(entityTuple);

        auto modelMatrix = Matrix4::CreateTranslationMatrix(transform.Position) *
                           Matrix4::CreateScaleMatrix(transform.Scale) *
                           Matrix4::CreateEulerAngleMatrixXYZ(transform.Rotation);
        auto normalMatrix = modelMatrix.Inverse().Transpose();
        auto mvpMatrix = viewProjectionMatrix * modelMatrix;
        // grabbing model might be better put elsewhere
        auto &modelData = m_graphicsManager->GetModel(model.modelName);

        // Queue the model in a thread-safe manner
        queuedModels.push_back({modelData, mvpMatrix});
    });
    auto boxColliderView = m_registry->CreateView<TransformComponent, BoxBoundsComponent>();
    auto sphereColliderView = m_registry->CreateView<TransformComponent, SphereBoundsComponent>();
    // Submit all queued models
    for (const auto &queuedModel : queuedModels)
    {
        Renderer::QueueModel(queuedModel.first, queuedModel.second);
    }
    Renderer::SubmitQueue();


}

void RenderSystem::LateRender()
{
    //  UI Rendering
}

void RenderSystem::Shutdown()
{
}

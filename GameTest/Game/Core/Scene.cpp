#include "stdafx.h"
#include "Scene.h"
#include <Game/Graphics/Camera.h>
#include <Game/Managers/GraphicsManager.h>
#include <Game/Math/Vector2.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <memory>
#include <Game/Managers/EventManager.h>

Scene::Scene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
             std::shared_ptr<PlayerManager> scoreManager)
    : m_registry(std::make_unique<Registry>()),
      m_camera(std::make_unique<Camera>(FVector3(0.0f, 10.0f, 8.0f), FVector3(0.0f, -1.0f, 0.0f), Quaternion(), 90.0f)),
      m_graphicsManager(graphicsManager),
      m_renderSystem(std::make_shared<RenderSystem>(m_registry.get(), m_graphicsManager.get(), m_camera.get())),
      m_eventManager(eventManager), m_playerManager(scoreManager)
{
}

void Scene::Init()
{
    // m_graphicsManager->LoadResources();
    m_renderSystem->Init();
    
}

void Scene::LateInit()
{
}

void Scene::Update(const float dt)
{

    m_renderSystem->Update();
}

void Scene::LateUpdate(const float dt)
{
}
void Scene::Render()
{
    m_renderSystem->Render();
}
void Scene::LateRender()
{
    m_renderSystem->LateRender();

}

void Scene::Shutdown()
{
}

void Scene::LateShutdown()
{
}

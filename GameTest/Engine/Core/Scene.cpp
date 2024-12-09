#include "stdafx.h"
#include "Scene.h"
#include "Engine/Core/Components.h"

Scene::Scene()
    : m_registry(std::make_shared<Registry>()), m_audioManager(std::make_shared<AudioManager>()),
      m_graphicsManager(std::make_shared<GraphicsManager>()),
      m_camera(std::make_shared<Camera>(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 90.0f)),
      m_renderSystem(m_registry.get(), m_graphicsManager.get(), m_camera.get()),
      m_collisionSystem(),
      m_physicsSystem(m_registry.get())
{
    m_collisionManager = std::make_shared<CollisionManager>();
}

void Scene::Init()
{
    m_graphicsManager->LoadResources();
    m_renderSystem.Init();
    m_collisionSystem.Init();
    m_physicsSystem.Init();
}

void Scene::LateInit()
{
}

void Scene::Update(const float dt)
{
    m_physicsSystem.Update(dt);
    m_collisionSystem.Update(dt);
}

void Scene::LateUpdate(const float dt)
{
}

void Scene::Render()
{
    m_renderSystem.Render();
}

void Scene::LateRender()
{
}

void Scene::Shutdown()
{
}

void Scene::LateShutdown()
{
}

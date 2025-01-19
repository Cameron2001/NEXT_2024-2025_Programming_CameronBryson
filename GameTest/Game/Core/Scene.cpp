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
      m_graphicsManager(graphicsManager), m_renderSystem(std::make_shared<RenderSystem>(m_registry.get(), m_graphicsManager.get(), m_camera.get())),
      m_collisionSystem(std::make_shared<CollisionSystem>(m_registry.get(), eventManager.get())), m_physicsSystem(std::make_shared<PhysicsSystem>(m_registry.get())), 
      m_particleSystem(std::make_shared<ParticleSystem>(m_registry.get(),m_eventManager.get())), m_cameraSystem(std::make_shared<CameraSystem>(m_registry.get(), scoreManager.get(), m_camera.get())), 
      m_entityFactory(std::make_shared<EntityFactory>(m_registry.get())), m_eventManager(eventManager), m_playerManager(scoreManager), m_uiSystem(std::make_shared<UISystem>(m_registry.get(), scoreManager.get()))
{
}

void Scene::Init()
{
    // m_graphicsManager->LoadResources();
    m_renderSystem->Init();
    m_collisionSystem->Init();
    m_physicsSystem->Init();
    m_particleSystem->EmitParticles(FVector2(0.50f, 0.5f), 10);
}

void Scene::LateInit()
{
}

void Scene::Update(const float dt)
{
    m_physicsSystem->Update(dt);
    m_collisionSystem->Update(dt);
    m_particleSystem->Update(dt);
    m_cameraSystem->Update(dt);
    m_uiSystem->Update(dt);
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
    m_particleSystem->Render();
}

void Scene::Shutdown()
{
}

void Scene::LateShutdown()
{
}

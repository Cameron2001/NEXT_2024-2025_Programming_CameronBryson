#include "stdafx.h"
#include "Scene.h"
#include <Game/Graphics/Camera.h>
#include <Game/Managers/AudioManager.h>
#include <Game/Managers/GraphicsManager.h>
#include <Game/Math/Vector2.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <memory>
#include <Game/Managers/EventManager.h>

Scene::Scene(std::shared_ptr<AudioManager> audioManager,
             std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager)
    : m_registry(std::make_unique<Registry>()),
      m_camera(std::make_unique<Camera>(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 1.0f, 0.0f), Quaternion(), 90.0f)),
      m_audioManager(audioManager), m_graphicsManager(graphicsManager),
      m_renderSystem(m_registry.get(), m_graphicsManager.get(), m_camera.get()), m_collisionSystem(m_registry.get()),
      m_physicsSystem(m_registry.get()), m_particleSystem(m_registry.get()), m_cameraSystem(m_registry.get(), m_camera.get()),
      m_entityFactory(m_registry.get())
{
}

void Scene::Init()
{
    //m_graphicsManager->LoadResources();
    m_renderSystem.Init();
    m_collisionSystem.Init();
    m_physicsSystem.Init();
    m_particleSystem.EmitParticles(FVector2(0.50f, 0.5f), 10);
}

void Scene::LateInit()
{
}

void Scene::Update(const float dt)
{
    m_physicsSystem.Update(dt);
    m_collisionSystem.Update(dt);
    m_particleSystem.Update(dt);
    m_cameraSystem.Update(dt);
    m_renderSystem.Update();
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
    m_particleSystem.Render();
}

void Scene::Shutdown()
{
}

void Scene::LateShutdown()
{
}

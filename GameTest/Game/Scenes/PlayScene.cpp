#include "stdafx.h"
#include "PlayScene.h"
#include "App/app.h"
#include <Game/Core/Event.h>

PlayScene::PlayScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                     std::shared_ptr<PlayerManager> scoreManager)
    : Scene(graphicsManager, eventManager, scoreManager),
      m_playerSystem(std::make_shared<PlayerSystem>(m_registry.get(), eventManager.get(), m_playerManager.get(),m_camera.get())),
      m_collisionSystem(std::make_shared<CollisionSystem>(m_registry.get(), eventManager.get())),
      m_particleSystem(std::make_shared<ParticleSystem>(m_registry.get(), eventManager.get())),
      m_entityFactory(std::make_shared<EntityFactory>(m_registry.get())),
      m_uiSystem(std::make_shared<UISystem>(m_registry.get(), m_playerManager.get()))
{
}

void PlayScene::Init()
{
    Scene::Init();

    m_playerSystem->Init();
    
    m_eventManager->AddListener<unsigned int, unsigned int>("CollisionEvent", m_playerSystem, &PlayerSystem::OnCollision);
    m_eventManager->AddListener<FVector2, int, FVector3>("EmitParticles", m_particleSystem, &ParticleSystem::EmitParticles);
    m_collisionSystem->Init();
    m_physicsSystem->Init();
    
}

void PlayScene::LateInit()
{
    Scene::LateInit();
    m_collisionSystem->LateInit();
}

void PlayScene::Update(const float dt)
{
    m_physicsSystem->Update(dt);
    m_collisionSystem->Update(dt);
    m_particleSystem->Update(dt);
    m_uiSystem->Update(dt);
    m_playerSystem->Update(dt);
    Scene::Update(dt);
}

void PlayScene::LateUpdate(const float dt)
{
    Scene::LateUpdate(dt);
}

void PlayScene::Render()
{
    Scene::Render();
}

void PlayScene::LateRender()
{
    Scene::LateRender();
    m_particleSystem->Render();
}

void PlayScene::Shutdown()
{
    Scene::Shutdown();
}

void PlayScene::LateShutdown()
{
    Scene::LateShutdown();
}



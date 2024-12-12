#include "stdafx.h"
#include "PlayScene.h"
#include "Game/GameComponents.h"
#include "Engine/Core/Event.h"
#include "Engine/Core/Threadpool.h"
Event<int> m_event;
Threadpool m_threadpool(4);

PlayScene::PlayScene() : Scene() , m_playerSystem(m_registry.get())
{
}

void PlayScene::Init()
{
    Scene::Init();
    m_playerSystem.Init();
    m_event.AddListener(this, &PlayScene::Test);
    auto player = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(player, FVector3(0.0f, 0.0f, -1.0f), FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(0.15f, 0.15f, 0.15f));
    m_registry->AddComponent<PlayerComponent>(player, 0.0000001);
    m_registry->AddComponent<ModelComponent>(player, "CubeOBJ");
    m_registry->AddComponent<RigidBodyComponent>(player);
    m_event.Notify(10);

    auto cube = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube, FVector3(0.1f, 0.1f, -3.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube, "CubeOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube);

    m_event.Notify(15);
    auto future = m_threadpool.QueueTask([this](int value) {
        this->Test(value);
    }, 9);

}

void PlayScene::LateInit()
{
    Scene::LateInit();
}

void PlayScene::Update(const float dt)
{
    Scene::Update(dt);

    m_playerSystem.Update(dt);
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
}

void PlayScene::Shutdown()
{
    Scene::Shutdown();
}

void PlayScene::LateShutdown()
{
    Scene::LateShutdown();
}

void PlayScene::Test(int value)
{

}

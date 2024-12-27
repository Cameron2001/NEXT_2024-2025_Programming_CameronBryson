#include "stdafx.h"
#include "PlayScene.h"
#include "Game/GameComponents.h"
#include "Engine/Core/Events.h"
#include "Engine/Core/Threadpool.h"
#include "App/app.h"
#include <Engine/Core/Event.h>

Threadpool m_threadpool(4);

PlayScene::PlayScene() : Scene(), m_playerSystem(m_registry.get())
{
}

void PlayScene::Init()
{
    Scene::Init();
    m_playerSystem.Init();
    auto self = shared_from_this();
    TestEvent.AddListener(self, &PlayScene::Test);
    TestEvent2.AddListener(self, &PlayScene::Test);
    auto player = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(player, FVector3(0.0f, 0.0f, -1.0f), FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(0.15f, 0.15f, 0.15f));
    m_registry->AddComponent<PlayerComponent>(player, 2.0f);
    m_registry->AddComponent<ModelComponent>(player, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(player);
    // m_event.Notify(10);

    auto cube = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube, FVector3(0.1f, 0.1f, -6.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube);

    auto cube2 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube2, FVector3(-0.1f, -0.1f, -3.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube2, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube2);

    auto cube3 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube3, FVector3(0.3f, -0.3f, -8.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube3, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube3);

    auto cube4 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube4, FVector3(0.4f, -0.2f, -4.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube4, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube4);

    auto cube5 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube5, FVector3(-0.5f, -0.8f, -10.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube5, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube5);

    auto cube6 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube6, FVector3(-0.5f, 0.6f, -5.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube6, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube6);

    auto cube7 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube7, FVector3(-0.9f, -0.9f, -6.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube7, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube7);

    auto cube8 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(cube8, FVector3(0.9f, 0.9f, -6.0f), FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube8, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube8);

    TestEvent.Notify(15);
    // auto future = m_threadpool.QueueTask([this](int value) { this->Test(value); }, 9);
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
    printf("Value: %d\n", value);
}

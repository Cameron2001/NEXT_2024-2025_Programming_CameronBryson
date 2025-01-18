#include "stdafx.h"
#include "PlayScene.h"
#include "Game/Core/Events.h"
#include "App/app.h"
#include <Game/Core/Event.h>

// Threadpool m_threadpool(4);

PlayScene::PlayScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                     std::shared_ptr<PlayerManager> scoreManager)
    : Scene(graphicsManager, eventManager,scoreManager), m_playerSystem(m_registry.get())
{
}

void PlayScene::Init()
{
    Scene::Init();
    m_playerSystem.Init();
    auto self = shared_from_this();
    TestEvent.AddListener(self, &PlayScene::Test);
    TestEvent2.AddListener(self, &PlayScene::Test);
    auto player1 = m_entityFactory.CreateGolfBallOne(FVector3(5.0f, 3.5f, -40.0f));
    auto player2 = m_entityFactory.CreateGolfBallTwo(FVector3(-5.0f, 3.5f, -40.0f));
    m_entityFactory.CreateArrow(player1);
    m_entityFactory.CreateArrow(player2);
    m_playerManager->SetPlayer1(player1);
    m_playerManager->SetPlayer2(player2);
    m_entityFactory.CreateFlag(FVector3(1.5f, 2.5f, -40.0f));
    auto player1ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(player1ScoreText, "Player1 Score:", FVector2(20.0f, 15.9f));

    auto player2ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(player2ScoreText, "Player2 Score:", FVector2(20.0f, 60.9f));

    m_uiSystem.SetScoreTextEntities(player1ScoreText, player2ScoreText);

    auto powerScaleText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(powerScaleText, "Power Scale:", FVector2(20.0f, 200.9f));
    m_arrowSystem.SetScaleTextEntity(powerScaleText);
    // auto cube = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube, FVector3(-1.5f, -0.8f, -20.0f), FVector3(0.0f, 0.0f, 0.0f),
    //                                              FVector3(1.0f, 1.0f, 1.0f));
    // m_registry->AddComponent<ModelComponent>(cube, "SphereOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube, 0.5, 0.5);
    // m_registry->AddComponent<ColliderComponent>(cube, ColliderType::Sphere, true);
    //// m_registry->AddComponent<BoxBoundsComponent>(cube, FVector3(1.0f, 1.0f, 1.0f));
    // m_registry->AddComponent<SphereBoundsComponent>(cube, 1.0f);

    // m_entityFactory.CreateDynamicBox(FVector3(-1.0f, 3.0f, -20.0f), FVector3(1.0f, 1.0f, 1.0f));
    // m_entityFactory.CreateDynamicBox(FVector3(-3.5f, 3.0f, -20.0f), FVector3(1.0f, 1.0f, 1.0f));

    // m_entityFactory.CreateDynamicSphere(FVector3(1.5f, 0.0f, -20.0f), 1.0f);
    m_entityFactory.CreateGrassBox(FVector3(0.0f, 0.0f, -50.0f), FVector3(1.0f, 1.0f, 1.0f));

    /*

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
    m_registry->AddComponent<TransformComponent>(cube5, FVector3(-0.5f, -0.8f, -10.0f),
    FVector3(20.0f, 15.0f, 10.0f),
                                                 FVector3(0.15f, 0.15f, 0.51f));
    m_registry->AddComponent<ModelComponent>(cube5, "ShipOBJ");
    m_registry->AddComponent<RigidBodyComponent>(cube5);*/

    // auto cube6 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube6, FVector3(-0.5f, 0.6f, -5.0f), FVector3(20.0f, 15.0f, 10.0f),
    //                                              FVector3(0.15f, 0.15f, 0.51f));
    // m_registry->AddComponent<ModelComponent>(cube6, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube6);

    // auto cube7 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube7, FVector3(-0.9f, -0.9f, -6.0f), FVector3(20.0f, 15.0f, 10.0f),
    //                                              FVector3(0.15f, 0.15f, 0.51f));
    // m_registry->AddComponent<ModelComponent>(cube7, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube7);

    // auto cube8 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube8, FVector3(0.9f, 0.9f, -6.0f), FVector3(20.0f, 15.0f, 10.0f),
    //                                              FVector3(0.15f, 0.15f, 0.51f));
    // m_registry->AddComponent<ModelComponent>(cube8, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube8);

    // TestEvent.Notify(15);
    //// auto future = m_threadpool.QueueTask([this](int value) { this->Test(value); }, 9);
    // auto cube9 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube9, FVector3(0.2f, 0.2f, -7.0f), FVector3(10.0f, 10.0f, 10.0f),
    //                                              FVector3(0.2f, 0.2f, 0.2f));
    // m_registry->AddComponent<ModelComponent>(cube9, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube9);

    // auto cube10 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube10, FVector3(-0.2f, 0.3f, -7.5f), FVector3(15.0f, 15.0f, 15.0f),
    //                                              FVector3(0.25f, 0.25f, 0.25f));
    // m_registry->AddComponent<ModelComponent>(cube10, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube10);

    // auto cube11 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube11, FVector3(0.5f, -0.5f, -8.5f), FVector3(30.0f, 30.0f, 30.0f),
    //                                              FVector3(0.3f, 0.3f, 0.3f));
    // m_registry->AddComponent<ModelComponent>(cube11, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube11);

    // auto cube12 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube12, FVector3(-0.3f, 0.4f, -9.0f), FVector3(25.0f, 25.0f, 25.0f),
    //                                              FVector3(0.35f, 0.35f, 0.35f));
    // m_registry->AddComponent<ModelComponent>(cube12, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube12);

    // auto cube13 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube13, FVector3(0.6f, -0.6f, -9.5f), FVector3(35.0f, 35.0f, 35.0f),
    //                                              FVector3(0.4f, 0.4f, 0.4f));
    // m_registry->AddComponent<ModelComponent>(cube13, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube13);

    // auto cube14 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube14, FVector3(-0.4f, 0.5f, -10.5f),
    // FVector3(40.0f, 40.0f, 40.0f),
    //                                              FVector3(0.45f, 0.45f, 0.45f));
    // m_registry->AddComponent<ModelComponent>(cube14, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube14);

    // auto cube15 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube15, FVector3(0.7f, -0.7f, -11.0f),
    // FVector3(45.0f, 45.0f, 45.0f),
    //                                              FVector3(0.5f, 0.5f, 0.5f));
    // m_registry->AddComponent<ModelComponent>(cube15, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube15);

    // auto cube16 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube16, FVector3(-0.5f, 0.6f, -11.5f),
    // FVector3(50.0f, 50.0f, 50.0f),
    //                                              FVector3(0.55f, 0.55f, 0.55f));
    // m_registry->AddComponent<ModelComponent>(cube16, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube16);

    // auto cube17 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube17, FVector3(0.8f, -0.8f, -12.0f),
    // FVector3(55.0f, 55.0f, 55.0f),
    //                                              FVector3(0.6f, 0.6f, 0.6f));
    // m_registry->AddComponent<ModelComponent>(cube17, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube17);

    // auto cube18 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube18, FVector3(-0.6f, 0.7f, -12.5f),
    // FVector3(60.0f, 60.0f, 60.0f),
    //                                              FVector3(0.65f, 0.65f, 0.65f));
    // m_registry->AddComponent<ModelComponent>(cube18, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube18);

    // auto cube19 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube19, FVector3(0.9f, -0.9f, -13.0f),
    // FVector3(65.0f, 65.0f, 65.0f),
    //                                              FVector3(0.7f, 0.7f, 0.7f));
    // m_registry->AddComponent<ModelComponent>(cube19, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube19);

    // auto cube20 = m_registry->CreateEntity();
    // m_registry->AddComponent<TransformComponent>(cube20, FVector3(-0.7f, 0.8f, -13.5f),
    // FVector3(70.0f, 70.0f, 70.0f),
    //                                              FVector3(0.75f, 0.75f, 0.75f));
    // m_registry->AddComponent<ModelComponent>(cube20, "ShipOBJ");
    // m_registry->AddComponent<RigidBodyComponent>(cube20);
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

#include "stdafx.h"
#include "LevelThreeScene.h"

LevelThreeScene::LevelThreeScene(std::shared_ptr<GraphicsManager> graphicsManager,
                                 std::shared_ptr<EventManager> eventManager,
                                 std::shared_ptr<PlayerManager> scoreManager)
    : PlayScene(graphicsManager, eventManager, scoreManager)
{
}

void LevelThreeScene::Init()
{
    BuildLevelThree();
    PlayScene::Init();
}

void LevelThreeScene::LateInit()
{
    PlayScene::LateInit();
}

void LevelThreeScene::Update(float dt)
{
    PlayScene::Update(dt);
}

void LevelThreeScene::LateUpdate(float dt)
{
    PlayScene::LateUpdate(dt);
}

void LevelThreeScene::Render()
{
    PlayScene::Render();
}

void LevelThreeScene::LateRender()
{
    PlayScene::LateRender();
}

void LevelThreeScene::Shutdown()
{
    PlayScene::Shutdown();
}

void LevelThreeScene::LateShutdown()
{
    PlayScene::LateShutdown();
}

void LevelThreeScene::BuildLevelThree()
{
    auto player1 = m_entityFactory->CreateGolfBallOne(FVector3(0, 10, 6));
    auto player2 = m_entityFactory->CreateGolfBallTwo(FVector3(-5.0f, 4.5f, -20.0f));
    auto hole = m_entityFactory->CreateStaticBox(FVector3(1.5f, 4.0f, -20.0f), FVector3(1.0f, 1.0f, 1.0f),
                                                 FVector3(0, 0, 0), FVector3(1, 1, 1));
    m_entityFactory->CreateArrow(player1);
    m_entityFactory->CreateArrow(player2);
    m_playerManager->SetPlayer1(player1);
    m_playerManager->SetPlayer2(player2);
    m_playerManager->SetHole(hole);
    m_entityFactory->CreateFlag(FVector3(1.5f, 2.5f, -20.0f));
    auto player1ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(player1ScoreText, "Player1 Score:", FVector2(20.0f, 15.9f));

    auto player2ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(player2ScoreText, "Player2 Score:", FVector2(20.0f, 60.9f));

    m_uiSystem->SetScoreTextEntities(player1ScoreText, player2ScoreText);

    auto powerScaleText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(powerScaleText, "Power Scale:", FVector2(20.0f, 200.9f));
    m_playerSystem->SetScaleTextEntity(powerScaleText);

    auto golfCourse = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(golfCourse, FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 01.5f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ModelComponent>(golfCourse, "GolfCourseOBJ");
    
}

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

    const int gridRows = 8;
    const int gridColumns = 6;
    const float spacing = 6.05f;

    FVector3 startPosition(-15.0f, 0.0f, -30.0f);
    FVector3 startPosition2(-15.0f, 7.0f, -100.0f);

    FVector3 firstGrassColor(0.0f, 0.7f, 0.0f);
    FVector3 secondGrassColor(0.7f, 0.7f, 0.7f);
    FVector3 bouncyBoxColor(0.0f, 0.0f, 1.0f);
    FVector3 sandBoxColor(1.0f, 1.0f, 0.0f);
    FVector3 iceBoxColor(0.0f, 1.0f, 1.0f);

    float grassGridElasticity = 0.65f;
    float grassGridStaticFriction = 0.8f;
    float grassGridDynamicFriction = 0.8f;

    float bouncyGridElasticity = 2.0f;
    float bouncyGridStaticFriction = 0.1f;
    float bouncyGridDynamicFriction = 0.1f;

    float sandGridElasticity = 0.2f;
    float sandGridStaticFriction = 0.99f;
    float sandGridDynamicFriction = 0.99f;

    float iceGridElasticity = 0.5f;
    float iceGridStaticFriction = 0.01f;
    float iceGridDynamicFriction = 0.01f;

    float boundaryGridElasticity = 0.7f;
    float boundaryGridStaticFriction = 0.7f;
    float boundaryGridDynamicFriction = 0.7f;

    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;

            FVector3 currentColor = (row + col) % 2 == 0 ? firstGrassColor : secondGrassColor;
            m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f), FVector3(0, 0, 0), currentColor);
        }
    }

    FVector3 largeColliderPosition(startPosition.x + (gridColumns * spacing) / 2.0f - spacing / 2.0f, startPosition.y,
                                   startPosition.z + ((gridRows)*spacing) / 2.0f - spacing / 2.0f);

    FVector3 largeColliderExtents((gridColumns * spacing) / 2.0f, 2.0f, ((gridRows - 1) * spacing) / 2.0f);

    m_entityFactory->CreateInvisibleBoxCollider(largeColliderPosition, largeColliderExtents, grassGridElasticity,
                                                grassGridStaticFriction, grassGridDynamicFriction);

    FVector3 borderColor(1.0f, 0.0f, 0.0f);
    float borderHeight = 5.0f;

    for (int row = 0; row < gridRows; ++row)
    {
        FVector3 leftBorderPos(startPosition.x - spacing, startPosition.y + borderHeight / 2.0f,
                               startPosition.z + row * spacing);
        m_entityFactory->CreateStaticBox(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0, 0, 0),
                                         borderColor);
        m_entityFactory->CreateInvisibleBoxCollider(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    boundaryGridElasticity, boundaryGridStaticFriction,
                                                    boundaryGridDynamicFriction);

        FVector3 rightBorderPos(startPosition.x + gridColumns * spacing, startPosition.y + borderHeight / 2.0f,
                                startPosition.z + row * spacing);
        m_entityFactory->CreateStaticBox(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0, 0, 0),
                                         borderColor);
        m_entityFactory->CreateInvisibleBoxCollider(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    boundaryGridElasticity, boundaryGridStaticFriction,
                                                    boundaryGridDynamicFriction);
    }

    m_entityFactory->CreateStaticBox(FVector3(0, 2.0f, -42), FVector3(12.0f, 3.0f, 6.0f), FVector3(-0.3f, 0.0f, 0.0f),
                                     FVector3(0.5f, 0.5f, 0.5f));
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(0, 2.0f, -42), FVector3(12.0f, 3.0f, 6.0f), 0.5f, 0.5f, 0.5f);

    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition2;
            position.x += col * spacing;
            position.z += row * spacing;

            FVector3 currentColor = (row + col) % 2 == 0 ? firstGrassColor : secondGrassColor;
            m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f), FVector3(0, 0, 0), currentColor);
        }
    }

    FVector3 largeColliderPosition2(startPosition2.x + (gridColumns * spacing) / 2.0f - spacing / 2.0f,
                                    startPosition2.y, startPosition2.z + ((gridRows)*spacing) / 2.0f - spacing / 2.0f);

    FVector3 largeColliderExtents2((gridColumns * spacing) / 2.0f, 2.0f, ((gridRows - 1) * spacing) / 2.0f);

    m_entityFactory->CreateInvisibleBoxCollider(largeColliderPosition2, largeColliderExtents2, grassGridElasticity,
                                                grassGridStaticFriction, grassGridDynamicFriction);

    for (int row = 0; row < gridRows; ++row)
    {
        FVector3 leftBorderPos(startPosition2.x - spacing, startPosition2.y + borderHeight / 2.0f,
                               startPosition2.z + row * spacing);
        m_entityFactory->CreateStaticBox(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0, 0, 0),
                                         borderColor);
        m_entityFactory->CreateInvisibleBoxCollider(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    boundaryGridElasticity, boundaryGridStaticFriction,
                                                    boundaryGridDynamicFriction);

        FVector3 rightBorderPos(startPosition2.x + gridColumns * spacing, startPosition2.y + borderHeight / 2.0f,
                                startPosition2.z + row * spacing);
        m_entityFactory->CreateStaticBox(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0, 0, 0),
                                         borderColor);
        m_entityFactory->CreateInvisibleBoxCollider(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    boundaryGridElasticity, boundaryGridStaticFriction,
                                                    boundaryGridDynamicFriction);
    }

    m_entityFactory->CreateStaticBox(FVector3(-6, 4, -12), FVector3(2, 2, 2), FVector3(0, 0, 0), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(-6, 4, -12), FVector3(2, 2, 2), bouncyGridElasticity,
                                                bouncyGridStaticFriction, bouncyGridDynamicFriction);

    m_entityFactory->CreateStaticBox(FVector3(6, 4, -12), FVector3(2, 2, 2), FVector3(0, 0, 0), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(6, 4, -12), FVector3(2, 2, 2), bouncyGridElasticity,
                                                bouncyGridStaticFriction, bouncyGridDynamicFriction);

    m_entityFactory->CreateStaticBox(FVector3(5, 2, -10), FVector3(2, 2, 2), FVector3(0, 0, 0), iceBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(5, 2, -10), FVector3(2, 2, 2), iceGridElasticity,
                                                iceGridStaticFriction, iceGridDynamicFriction);

    m_entityFactory->CreateStaticBox(FVector3(-5, 2, -10), FVector3(2, 2, 2), FVector3(0, 0, 0), sandBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(-5, 2, -10), FVector3(2, 2, 2), sandGridElasticity,
                                                sandGridStaticFriction, sandGridDynamicFriction);

    m_entityFactory->CreateBouncySphere(FVector3(-2, 2, -5), 1.0f);

    /*m_entityFactory->CreateStaticBoxGrid(5, 5, FVector3(0, 5, -20), 6.0f, FVector3(2.0f, 2.0f, 2.0f), bouncyBoxColor,
                                         bouncyGridElasticity, bouncyGridStaticFriction, bouncyGridDynamicFriction);*/
}

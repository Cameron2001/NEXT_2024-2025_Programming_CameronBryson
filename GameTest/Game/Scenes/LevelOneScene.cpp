#include "stdafx.h"
#include "LevelOneScene.h"

LevelOneScene::LevelOneScene(std::shared_ptr<GraphicsManager> graphicsManager,
                             std::shared_ptr<EventManager> eventManager, std::shared_ptr<PlayerManager> scoreManager)
    : PlayScene(graphicsManager, eventManager, scoreManager)
{
}

void LevelOneScene::Init()
{
    BuildLevelOne();
    PlayScene::Init();

}

void LevelOneScene::LateInit()
{
    PlayScene::LateInit();
}

void LevelOneScene::Update(float dt)
{
    PlayScene::Update(dt);
}

void LevelOneScene::LateUpdate(float dt)
{
    PlayScene::LateUpdate(dt);
}

void LevelOneScene::Render()
{
    PlayScene::Render();
}

void LevelOneScene::LateRender()
{
    PlayScene::LateRender();
}

void LevelOneScene::Shutdown()
{
    PlayScene::Shutdown();
}

void LevelOneScene::LateShutdown()
{
    PlayScene::LateShutdown();
}

void LevelOneScene::BuildLevelOne()
{
    auto player1 = m_entityFactory->CreateGolfBallOne(FVector3(0, 10, 6));
    auto player2 = m_entityFactory->CreateGolfBallTwo(FVector3(-5.0f, 4.5f, -20.0f));
    auto hole = m_entityFactory->CreateStaticBox(FVector3(1.5f, 4.0f, -20.0f), FVector3(1.0f, 1.0f, 1.0f));
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

    const int gridRows = 8;     // Number of rows
    const int gridColumns = 6;   // Number of columns
    const float spacing = 6.05f; // Distance between blocks

    FVector3 startPosition(-10.0f, 0.0f, -30.0f);

    FVector3 firstColor(0.0f, 0.7f, 0.0f);
    FVector3 secondColor(0.7f, 0.7f, 0.7f);

    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;

            FVector3 currentColor = (row + col) % 2 == 0 ? firstColor : secondColor;

            m_entityFactory->CreateGrassBox(position, FVector3(3.0f, 2.0f, 3.0f), currentColor);
        }
    }


    FVector3 largeColliderPosition =
        FVector3(startPosition.x + (gridColumns * spacing) / 2.0f - spacing / 2.0f, startPosition.y,
                 startPosition.z + (gridRows * spacing) / 2.0f - spacing / 2.0f);

    FVector3 largeColliderExtents = FVector3((gridColumns * spacing) / 2.0f,
                                             2.0f,
                                             (gridRows * spacing) / 2.0f);

    m_entityFactory->CreateInvisibleBoxCollider(largeColliderPosition, largeColliderExtents);


    FVector3 borderColor(1.0f, 0.0f, 0.0f); // Red color
    float borderHeight = 5.0f;              

    for (int row = 0; row < gridRows; ++row)
    {
        FVector3 leftBorderPos = FVector3(startPosition.x - spacing, 
                                          startPosition.y + borderHeight / 2.0f, startPosition.z + row * spacing);
        m_entityFactory->CreateBorder(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f), borderColor);

        FVector3 rightBorderPos =
            FVector3(startPosition.x + gridColumns * spacing, 
                     startPosition.y + borderHeight / 2.0f, startPosition.z + row * spacing);
        m_entityFactory->CreateBorder(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f), borderColor);
    }
    //this is the ramp
    //Its angled slightly up
    m_entityFactory->CreateStaticBox(FVector3(0, 2.0, -42), FVector3(12, 3, 6), FVector3(-0.3, 0.0, 0.0));
    
    FVector3 startPosition2(-10.0f, 7.0f, -100.0f);


    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition2;
            position.x += col * spacing;
            position.z += row * spacing;

            FVector3 currentColor = (row + col) % 2 == 0 ? firstColor : secondColor;

            m_entityFactory->CreateGrassBox(position, FVector3(3.0f, 2.0f, 3.0f), currentColor);
        }
    }
    // Create collider for the second grid
    FVector3 largeColliderPosition2 =
        FVector3(startPosition2.x + (gridColumns * spacing) / 2.0f - spacing / 2.0f, startPosition2.y,
                 startPosition2.z + (gridRows * spacing) / 2.0f - spacing / 2.0f);

    FVector3 largeColliderExtents2 = FVector3((gridColumns * spacing) / 2.0f, 2.0f, (gridRows * spacing) / 2.0f);

    m_entityFactory->CreateInvisibleBoxCollider(largeColliderPosition2, largeColliderExtents2);
    
    m_entityFactory->CreateBouncyBox(FVector3(0, 2, -10), FVector3(2, 2, 2));
    m_entityFactory->CreateIceBox(FVector3(5, 2, -10), FVector3(2, 2, 2));
    m_entityFactory->CreateSandBox(FVector3(-5, 2, -10), FVector3(2, 2, 2));
   
    
    
}

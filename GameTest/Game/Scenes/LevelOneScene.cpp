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
    auto player1 = m_entityFactory->CreateGolfBallOne(FVector3(5.0f, 4.5f, -20.0f));
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

    const int gridRows = 6;      // Number of rows
    const int gridColumns = 6;   // Number of columns
    const float spacing = 6.02f; // Distance between blocks

    // Starting position for the grid
    FVector3 startPosition(-10.0f, 0.0f, -20.0f);

    // Define two colors for alternating
    FVector3 firstColor(0.0f, 0.7f, 0.0f);
    FVector3 secondColor(0.7f, 0.7f, 0.7f);

    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;

            // Determine the color based on row and column indices
            FVector3 currentColor = (row + col) % 2 == 0 ? firstColor : secondColor;

            m_entityFactory->CreateGrassBox(position, FVector3(3.0f, 2.0f, 3.0f), currentColor);
        }
    }

    // Create red borders around the grid
    FVector3 borderColor(1.0f, 0.0f, 0.0f); // Red color

    float gridWidth = gridColumns * spacing;
    float gridDepth = gridRows * spacing;

    m_entityFactory->CreateBorder(FVector3(-5.2,5,0),FVector3(1,2,10), borderColor);

    
    
}

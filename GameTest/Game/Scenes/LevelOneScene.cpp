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
    BuildPlayers();
    BuildUI();
    BuildGrassArea();
    BuildGrassBoundaries();
    BuildIceArea();
    BuildIceBoundaries();
    BuildBouncyBoxes();
    BuildRamps();
    BuildCentralGrassArea();
    BuildSandBorders();
}

void LevelOneScene::BuildPlayers()
{
    auto player1 = m_entityFactory->CreateGolfBallOne(FVector3(6.0f, 3.5f, 10.0f));
    auto player2 = m_entityFactory->CreateGolfBallTwo(FVector3(-6.0f, 3.5f, 10.0f));
    m_entityFactory->CreateArrow(player1);
    m_entityFactory->CreateArrow(player2);
    m_playerManager->SetPlayer1(player1);
    m_playerManager->SetPlayer2(player2);

    auto hole = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(hole, FVector3(-4.0f, 10.0f, -160.0f), FVector3(0.0f, 0.5f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ColliderComponent>(hole, ColliderType::Box, false, 0.8f, 0.5f);
    m_registry->AddComponent<BoxBoundsComponent>(hole, FVector3(1.0f, 1.0f, 1.0f));
    m_playerManager->SetHole(hole);
    m_entityFactory->CreateFlag(FVector3(-4.0f, 6.0f, -160.0f));
}

void LevelOneScene::BuildUI()
{
    auto player1ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(player1ScoreText, "Player1 Score:", FVector2(20.0f, 100.0f));
    auto player2ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(player2ScoreText, "Player2 Score:", FVector2(20.0f, 50.0f));
    m_uiSystem->SetScoreTextEntities(player1ScoreText, player2ScoreText);

    auto powerScaleText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(powerScaleText, "Power Scale:", FVector2(20.0f, 200.0f));
    m_playerSystem->SetScaleTextEntity(powerScaleText);
}

void LevelOneScene::BuildGrassArea()
{
    const int gridRows = 8;
    const int gridColumns = 6;
    const float spacing = 6.05f;
    FVector3 startPosition(-15.0f, 0.0f, -29.25f);
    FVector3 firstGrassColor(0.0f, 0.7f, 0.0f);
    FVector3 secondGrassColor(0.7f, 0.7f, 0.7f);
    float grassGridElasticity = 0.5f;
    float grassGridFriction = 0.8f;

    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;
            FVector3 currentColor = (row + col) % 2 == 0 ? firstGrassColor : secondGrassColor;
            m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                             currentColor);
        }
    }

    FVector3 grassColliderPosition(startPosition.x + ((gridColumns - 1) * spacing) / 2.0f, startPosition.y,
                                   startPosition.z + ((gridRows - 1) * spacing) / 2.0f);
    FVector3 grassColliderExtents((gridColumns * spacing) / 2.0f, 2.0f, (gridRows * spacing) / 2.0f);
    m_entityFactory->CreateInvisibleBoxCollider(grassColliderPosition, grassColliderExtents, FVector3(0.0f, 0.0f, 0.0f),
                                                grassGridElasticity, grassGridFriction);
}

void LevelOneScene::BuildGrassBoundaries()
{
    const int gridRows = 8;
    const int gridColumns = 6;
    const float spacing = 6.05f;
    float borderHeight = 3.0f;
    FVector3 startPosition(-15.0f, 0.0f, -29.25f);
    float leftBorderX = startPosition.x - spacing;
    float rightBorderX = startPosition.x + gridColumns * spacing;
    float borderY = startPosition.y + borderHeight / 2.0f;
    float boundaryGridElasticity = 0.3f;
    float boundaryGridFriction = 0.3f;

    for (int row = 0; row < gridRows; ++row)
    {
        FVector3 leftBorderPos(leftBorderX, borderY, startPosition.z + row * spacing);
        CreateBoundary(leftBorderPos, borderHeight, boundaryGridElasticity, boundaryGridFriction);
        FVector3 rightBorderPos(rightBorderX, borderY, startPosition.z + row * spacing);
        CreateBoundary(rightBorderPos, borderHeight, boundaryGridElasticity, boundaryGridFriction);
    }
}

void LevelOneScene::BuildIceArea()
{
    const int gridRows = 8;
    const int gridColumns = 6;
    const float spacing = 6.05f;
    FVector3 startPosition(-15.0f, 2.9f, -89.0f);
    FVector3 iceBoxColor(0.0f, 1.0f, 1.0f);
    float iceGridElasticity = 0.5f;
    float iceGridFriction = 0.2f;

    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;
            m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                             iceBoxColor);
        }
    }

    FVector3 iceColliderPosition(startPosition.x + ((gridColumns - 1) * spacing) / 2.0f, startPosition.y,
                                 startPosition.z + ((gridRows - 1) * spacing) / 2.0f);
    FVector3 iceColliderExtents((gridColumns * spacing) / 2.0f, 2.0f, (gridRows * spacing) / 2.0f);
    m_entityFactory->CreateInvisibleBoxCollider(iceColliderPosition, iceColliderExtents, FVector3(0.0f, 0.0f, 0.0f),
                                                iceGridElasticity, iceGridFriction);
}

void LevelOneScene::BuildIceBoundaries()
{
    const int gridRows = 8;
    const int gridColumns = 6;
    const float spacing = 6.05f;
    float borderHeight = 3.0f;
    FVector3 startPosition(-15.0f, 2.9f, -89.0f);
    float leftBorderX = startPosition.x - spacing;
    float rightBorderX = startPosition.x + gridColumns * spacing;
    float borderY = startPosition.y + borderHeight / 2.0f;
    float boundaryGridElasticity = 0.3f;
    float boundaryGridFriction = 0.3f;

    for (int row = 0; row < gridRows; ++row)
    {
        FVector3 leftBorderPos(leftBorderX, borderY, startPosition.z + row * spacing);
        CreateBoundary(leftBorderPos, borderHeight, boundaryGridElasticity, boundaryGridFriction);
        FVector3 rightBorderPos(rightBorderX, borderY, startPosition.z + row * spacing);
        CreateBoundary(rightBorderPos, borderHeight, boundaryGridElasticity, boundaryGridFriction);
    }
}

void LevelOneScene::BuildBouncyBoxes()
{
    FVector3 bouncyBoxColor(0.0f, 0.0f, 1.0f);
    float bouncyGridElasticity = 3.0f;
    float bouncyGridFriction = 0.1f;

    m_entityFactory->CreateStaticBox(FVector3(0.0f, 4.0f, -20.0f), FVector3(2.0f, 2.0f, 4.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(0.0f, 4.0f, -20.0f), FVector3(2.0f, 2.0f, 4.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity, bouncyGridFriction);

    m_entityFactory->CreateStaticBox(FVector3(-8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(-8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity, bouncyGridFriction);

    m_entityFactory->CreateStaticBox(FVector3(8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity, bouncyGridFriction);
}

void LevelOneScene::BuildRamps()
{
    FVector3 rampPosition1(0.0f, 2.5f, -38.0f);
    m_entityFactory->CreateStaticBox(rampPosition1, FVector3(18.5f, 1.0f, 6.0f), FVector3(0.2f, 0.0f, 0.0f),
                                     FVector3(0.5f, 0.5f, 0.5f));
    m_entityFactory->CreateInvisibleBoxCollider(rampPosition1, FVector3(18.5f, 1.0f, 6.0f), FVector3(-0.2f, 0.0f, 0.0f),
                                                0.2f, 0.5f);

    FVector3 rampPosition2(0.0f, 5.0f, -97.5f);
    m_entityFactory->CreateStaticBox(rampPosition2, FVector3(18.5f, 1.0f, 6.0f), FVector3(0.2f, 0.0f, 0.0f),
                                     FVector3(0.5f, 0.5f, 0.5f));
    m_entityFactory->CreateInvisibleBoxCollider(rampPosition2, FVector3(18.5f, 1.0f, 6.0f), FVector3(-0.2f, 0.0f, 0.0f),
                                                0.2f, 0.5f);
}

void LevelOneScene::BuildCentralGrassArea()
{
    const int centralGrassRows = 8;
    const int centralGrassColumns = 8;
    const int sandBorderWidth = 1;
    const float spacing = 6.05f;
    FVector3 sandStartPosition(20.0f, 5.25f, -145.0f);
    FVector3 centralGrassStartPosition = sandStartPosition;
    centralGrassStartPosition.x -= ((centralGrassColumns + 2 * sandBorderWidth) * spacing) / 2.0f;
    centralGrassStartPosition.z -= ((centralGrassRows + 2 * sandBorderWidth) * spacing) / 2.0f;
    FVector3 firstGrassColor(0.0f, 0.7f, 0.0f);
    FVector3 secondGrassColor(0.7f, 0.7f, 0.7f);
    float grassGridElasticity = 0.5f;
    float grassGridFriction = 0.8f;

    for (int row = 0; row < centralGrassRows; ++row)
    {
        for (int col = 0; col < centralGrassColumns; ++col)
        {
            FVector3 position = centralGrassStartPosition;
            position.x += col * spacing;
            position.z += row * spacing;
            FVector3 currentColor = (row + col) % 2 == 0 ? firstGrassColor : secondGrassColor;
            m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                             currentColor);
        }
    }

    FVector3 centralGrassColliderPosition(centralGrassStartPosition.x + ((centralGrassColumns - 1) * spacing) / 2.0f,
                                          centralGrassStartPosition.y,
                                          centralGrassStartPosition.z + ((centralGrassRows - 1) * spacing) / 2.0f);
    FVector3 centralGrassColliderExtents((centralGrassColumns * spacing) / 2.0f, 2.0f,
                                         (centralGrassRows * spacing) / 2.0f);
    m_entityFactory->CreateInvisibleBoxCollider(centralGrassColliderPosition, centralGrassColliderExtents,
                                                FVector3(0.0f, 0.0f, 0.0f), grassGridElasticity, grassGridFriction);
}

void LevelOneScene::BuildSandBorders()
{
    const int centralGrassRows = 8;
    const int centralGrassColumns = 8;
    const int sandBorderWidth = 1;
    const float spacing = 6.05f;
    FVector3 sandStartPosition(20.0f, 5.25f, -145.0f);
    FVector3 centralGrassStartPosition = sandStartPosition;
    centralGrassStartPosition.x -= ((centralGrassColumns + 2 * sandBorderWidth) * spacing) / 2.0f;
    centralGrassStartPosition.z -= ((centralGrassRows + 2 * sandBorderWidth) * spacing) / 2.0f;
    FVector3 sandBordersStartPosition = centralGrassStartPosition;
    sandBordersStartPosition.x -= sandBorderWidth * spacing;
    sandBordersStartPosition.z -= sandBorderWidth * spacing;
    int totalSandRows = centralGrassRows + 2 * sandBorderWidth;
    int totalSandColumns = centralGrassColumns + 2 * sandBorderWidth;
    FVector3 sandBoxColor(1.0f, 1.0f, 0.0f);
    float sandGridElasticity = 0.1f;
    float sandGridFriction = 0.9f;

    for (int row = 0; row < totalSandRows; ++row)
    {
        for (int col = 0; col < totalSandColumns; ++col)
        {
            if (row < sandBorderWidth || row >= (totalSandRows - sandBorderWidth) || col < sandBorderWidth ||
                col >= (totalSandColumns - sandBorderWidth))
            {
                FVector3 position = sandBordersStartPosition;
                position.x += col * spacing;
                position.z += row * spacing;
                m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                                 sandBoxColor);
                m_entityFactory->CreateInvisibleBoxCollider(position, FVector3(3.0f, 2.0f, 3.0f),
                                                            FVector3(0.0f, 0.0f, 0.0f), sandGridElasticity,
                                                            sandGridFriction);
            }
        }
    }
}

void LevelOneScene::CreateBoundary(const FVector3 &position, float height, float elasticity, float friction)
{
    FVector3 size(3.0f, height, 3.0f);
    FVector3 rotation(0.0f, 0.0f, 0.0f);
    FVector3 color(1.0f, 0.0f, 0.0f);
    m_entityFactory->CreateStaticBox(position, size, rotation, color);
    m_entityFactory->CreateInvisibleBoxCollider(position, size, rotation, elasticity, friction);
}

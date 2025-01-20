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
    auto player1 = m_entityFactory->CreateGolfBallOne(FVector3(0.0f, 5.0f, 6.0f));
    auto player2 = m_entityFactory->CreateGolfBallTwo(FVector3(-5.0f, 5.5f, -38.0f));
    auto hole = m_entityFactory->CreateStaticBox(FVector3(1.5f, 4.0f, -20.0f), FVector3(1.0f, 1.0f, 1.0f),
                                                 FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ColliderComponent>(hole, ColliderType::Box, false, true, 0.8f, 0.5f, 0.7f);
    m_registry->AddComponent<BoxBoundsComponent>(hole, FVector3(1.0f, 1.0f, 1.0f));
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

    float borderHeight = 3.0f; // Set borderHeight to 3.0f for proper visibility

    FVector3 startPosition(-15.0f, 0.0f, -29.25f);
    FVector3 startPosition2(-15.0f, 2.9f, -89.0f);

    FVector3 firstGrassColor(0.0f, 0.7f, 0.0f);
    FVector3 secondGrassColor(0.7f, 0.7f, 0.7f);
    FVector3 bouncyBoxColor(0.0f, 0.0f, 1.0f);
    FVector3 sandBoxColor(1.0f, 1.0f, 0.0f);
    FVector3 iceBoxColor(0.0f, 1.0f, 1.0f);

    float grassGridElasticity = 0.5f;
    float grassGridStaticFriction = 0.7f;
    float grassGridDynamicFriction = 0.f;

    float bouncyGridElasticity = 2.0f;
    float bouncyGridStaticFriction = 0.1f;
    float bouncyGridDynamicFriction = 0.1f;

    float sandGridElasticity = 0.2f;
    float sandGridStaticFriction = 0.99f;
    float sandGridDynamicFriction = 0.99f;

    float iceGridElasticity = 0.5f;
    float iceGridStaticFriction = 0.01f;
    float iceGridDynamicFriction = 0.01f;

    float boundaryGridElasticity = 0.3f;
    float boundaryGridStaticFriction = 0.3f;
    float boundaryGridDynamicFriction = 0.3f;

    const float SAND_BLOCK_HEIGHT = 2.0f;
    const float SAND_COLLIDER_Y_EXTENT = SAND_BLOCK_HEIGHT / 2.0f; // 1.0f

    // --------------------
    // 3. Create Grass Grid
    // --------------------
    for (int row = 0; row < gridRows; ++row) // Start from row 1 to exclude sand row
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;

            FVector3 currentColor = (row + col) % 2 == 0 ? firstGrassColor : secondGrassColor;

            Entity box = m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f),
                                                          FVector3(0.0f, 0.0f, 0.0f), currentColor);
            // Removed individual ColliderComponent and BoxBoundsComponent
        }
    }

    // Calculate Grass Collider Position and Extents Directly
    FVector3 grassColliderPosition(startPosition.x + ((gridColumns-1) * spacing) / 2.0f, 
                                   startPosition.y,
                                   startPosition.z + ((gridRows-1) * spacing) / 2.0f 
    );

    FVector3 grassColliderExtents(
        ((gridColumns) * spacing) / 2.0f, // Half width plus buffer
        2.0f,                                        // Y extent (matches block height)
        ((gridRows) * spacing) / 2.0f     // Half depth plus buffer (excluding sand row)
    );

    // Create Grass Collider
    m_entityFactory->CreateInvisibleBoxCollider(grassColliderPosition, grassColliderExtents, FVector3(0.0f, 0.0f, 0.0f),
                                                grassGridElasticity, grassGridStaticFriction, grassGridDynamicFriction);


    // Add Visible Borders for First Grid
    float leftBorderX = startPosition.x - spacing;
    float rightBorderX = startPosition.x + gridColumns * spacing;

    float borderY = startPosition.y + borderHeight / 2.0f;

    float borderStartZ = startPosition.z;
    float borderEndZ = startPosition.z + (gridRows - 1) * spacing;

    for (int row = 0; row < gridRows; ++row)
    {
        // Left Border
        FVector3 leftBorderPos(leftBorderX, borderY, startPosition.z + row * spacing);
        m_entityFactory->CreateStaticBox(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                         FVector3(1.0f, 0.0f, 0.0f)); // Red color
        m_entityFactory->CreateInvisibleBoxCollider(leftBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    FVector3(0.0f, 0.0f, 0.0f), boundaryGridElasticity,
                                                    boundaryGridStaticFriction, boundaryGridDynamicFriction);

        // Right Border
        FVector3 rightBorderPos(rightBorderX, borderY, startPosition.z + row * spacing);
        m_entityFactory->CreateStaticBox(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                         FVector3(1.0f, 0.0f, 0.0f)); // Red color
        m_entityFactory->CreateInvisibleBoxCollider(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    FVector3(0.0f, 0.0f, 0.0f), boundaryGridElasticity,
                                                    boundaryGridStaticFriction, boundaryGridDynamicFriction);
    }


    // Second Grid - Entirely Ice
    for (int row = 0; row < gridRows; ++row)
    {
        for (int col = 0; col < gridColumns; ++col)
        {
            FVector3 position = startPosition2;
            position.x += col * spacing;
            position.z += row * spacing;

            FVector3 currentColor = iceBoxColor;
            float elasticity = iceGridElasticity;
            float staticFriction = iceGridStaticFriction;
            float dynamicFriction = iceGridDynamicFriction;

            Entity box = m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f),
                                                          FVector3(0.0f, 0.0f, 0.0f), currentColor);

            // Removed individual ColliderComponent and BoxBoundsComponent
        }
    }

    FVector3 iceColliderPosition =
        FVector3(startPosition2.x + (gridColumns * spacing) / 2.0f - spacing / 2.0f, // X position remains the same
                 startPosition2.y,                                                   // Y position remains the same
                 startPosition2.z + ((gridRows - 1) * spacing) / 2.0f                // Adjusted Z position
        );

    FVector3 iceColliderExtents = FVector3((gridColumns * spacing) / 2.0f,   // X extents remain the same
                                           2.0f,                             // Y extents remain the same
                                           ((gridRows - 1) * spacing) / 2.0f // Adjusted Z extents
    );

    m_entityFactory->CreateInvisibleBoxCollider(iceColliderPosition, iceColliderExtents, FVector3(0.0f, 0.0f, 0.0f),
                                                iceGridElasticity, iceGridStaticFriction, iceGridDynamicFriction);

    // Add Visible Borders for Second Grid
    float leftBorderX2 = startPosition2.x - spacing;
    float rightBorderX2 = startPosition2.x + gridColumns * spacing;

    float borderY2 = startPosition2.y + borderHeight / 2.0f;

    float borderStartZ2 = startPosition2.z;
    float borderEndZ2 = startPosition2.z + (gridRows - 1) * spacing;

    for (int row = 0; row < gridRows; ++row)
    {
        // Left Border
        FVector3 leftBorderPos2(leftBorderX2, borderY2, startPosition2.z + row * spacing);
        m_entityFactory->CreateStaticBox(leftBorderPos2, FVector3(3.0f, borderHeight, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                         FVector3(1.0f, 0.0f, 0.0f)); // Red color
        m_entityFactory->CreateInvisibleBoxCollider(leftBorderPos2, FVector3(3.0f, borderHeight, 3.0f),
                                                    FVector3(0.0f, 0.0f, 0.0f), boundaryGridElasticity,
                                                    boundaryGridStaticFriction, boundaryGridDynamicFriction);

        // Right Border
        FVector3 rightBorderPos2(rightBorderX2, borderY2, startPosition2.z + row * spacing);
        m_entityFactory->CreateStaticBox(rightBorderPos2, FVector3(3.0f, borderHeight, 3.0f),
                                         FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 0.0f, 0.0f)); // Red color
        m_entityFactory->CreateInvisibleBoxCollider(rightBorderPos2, FVector3(3.0f, borderHeight, 3.0f),
                                                    FVector3(0.0f, 0.0f, 0.0f), boundaryGridElasticity,
                                                    boundaryGridStaticFriction, boundaryGridDynamicFriction);
    }


    // Additional Entities
    //Ramp
    m_entityFactory->CreateStaticBox(FVector3(0.0f, 2.5f, -38.0f), FVector3(18.5f, 1.0f, 6.0f),
                                     FVector3(-0.2f, 0.0f, 0.0f), FVector3(0.5f, 0.5f, 0.5f));
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(0.0f, 2.5f, -38.0f), FVector3(18.5f, 1.0f, 6.0f),
                                                FVector3(-0.2f, 0.0f, 0.0f), 0.2f, 0.2f, 0.5f);

    m_entityFactory->CreateStaticBox(FVector3(-6.0f, 4.0f, -12.0f), FVector3(2.0f, 2.0f, 2.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(-6.0f, 4.0f, -12.0f), FVector3(2.0f, 2.0f, 2.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity,
                                                bouncyGridStaticFriction, bouncyGridDynamicFriction);

    m_entityFactory->CreateStaticBox(FVector3(6.0f, 4.0f, -12.0f), FVector3(2.0f, 2.0f, 2.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(6.0f, 4.0f, -12.0f), FVector3(2.0f, 2.0f, 2.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity,
                                                bouncyGridStaticFriction, bouncyGridDynamicFriction);

    m_entityFactory->CreateStaticBox(FVector3(5.0f, 2.0f, -10.0f), FVector3(2.0f, 2.0f, 2.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), iceBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(5.0f, 2.0f, -10.0f), FVector3(2.0f, 2.0f, 2.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), iceGridElasticity, iceGridStaticFriction,
                                                iceGridDynamicFriction);

    m_entityFactory->CreateStaticBox(FVector3(-5.0f, 2.0f, -10.0f), FVector3(2.0f, 2.0f, 2.0f),
                                     FVector3(0.0f, 0.0f, 0.0f), sandBoxColor);
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(-5.0f, 2.0f, -10.0f), FVector3(2.0f, 2.0f, 2.0f),
                                                FVector3(0.0f, 0.0f, 0.0f), sandGridElasticity, sandGridStaticFriction,
                                                sandGridDynamicFriction);

    m_entityFactory->CreateBouncySphere(FVector3(-2.0f, 2.0f, -5.0f), 1.0f);

    /*m_entityFactory->CreateStaticBoxGrid(5, 5, FVector3(0.0f, 5.0f, -20.0f), 6.0f, FVector3(2.0f, 2.0f, 2.0f),
       bouncyBoxColor, bouncyGridElasticity, grassGridStaticFriction, grassGridDynamicFriction);*/

    // Adjusted Ramp Positioning
    FVector3 rampStartPosition(0, 5, -92);

    // Create Ramp as a Single Sloped Piece
    m_entityFactory->CreateStaticBox(rampStartPosition, FVector3(18.5f, 1.0f, 6.0f), // Dimensions
                                     FVector3(-0.2f, 0.0f, 0.0f),                    // Rotation
                                     FVector3(0.5f, 0.5f, 0.5f)                      // Color
    );

    // Create Collider for Ramp
    m_entityFactory->CreateInvisibleBoxCollider(rampStartPosition,
                                                FVector3(18.5f, 1.0f, 6.0f), // Extents matching the ramp dimensions
                                                FVector3(-0.2f, 0.0f, 0.0f), // Rotation
                                                0.2f, 0.2f, 0.5f             // Elasticity and friction
    );

}

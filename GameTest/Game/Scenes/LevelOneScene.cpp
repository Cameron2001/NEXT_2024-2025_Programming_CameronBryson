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
    auto player1 = m_entityFactory->CreateGolfBallOne(FVector3(-2.0f, 10.5f, -150.0f));
    auto player2 = m_entityFactory->CreateGolfBallTwo(FVector3(-5.0f, 10.5f, -150.0f));
    auto hole = m_entityFactory->CreateStaticBox(FVector3(-4.0f, 10.0f, -160.0f), FVector3(0.3f, 3.0f, 0.3f),
                                                 FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ColliderComponent>(hole, ColliderType::Box, false, 0.8f, 0.5f);
    m_registry->AddComponent<BoxBoundsComponent>(hole, FVector3(1.0f, 1.0f, 1.0f));
    m_entityFactory->CreateArrow(player1);
    m_entityFactory->CreateArrow(player2);
    m_playerManager->SetPlayer1(player1);
    m_playerManager->SetPlayer2(player2);
    m_playerManager->SetHole(hole);
    m_entityFactory->CreateFlag(FVector3(-4.0f, 10.0f, -160.0f));

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
    float grassGridFriction = 0.8f;

    float bouncyGridElasticity = 2.0f;
    float bouncyGridFriction = 0.1f;

    float sandGridElasticity = 0.1f;
    float sandGridFriction = 0.9f;

    float iceGridElasticity = 0.5f;
    float iceGridFriction = 0.2f;

    float boundaryGridElasticity = 0.3f;
    float boundaryGridFriction = 0.3f;

    const float SAND_BLOCK_HEIGHT = 2.0f;
    const float SAND_COLLIDER_Y_EXTENT = SAND_BLOCK_HEIGHT / 2.0f; // 1.0f

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
    FVector3 grassColliderPosition(startPosition.x + ((gridColumns - 1) * spacing) / 2.0f, startPosition.y,
                                   startPosition.z + ((gridRows - 1) * spacing) / 2.0f);

    FVector3 grassColliderExtents(((gridColumns)*spacing) / 2.0f, // Half width plus buffer
                                  2.0f,                           // Y extent (matches block height)
                                  ((gridRows)*spacing) / 2.0f     // Half depth plus buffer (excluding sand row)
    );

    // Create Grass Collider
    m_entityFactory->CreateInvisibleBoxCollider(grassColliderPosition, grassColliderExtents, FVector3(0.0f, 0.0f, 0.0f),
                                                grassGridElasticity, grassGridFriction);

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
                                                    boundaryGridFriction);

        // Right Border
        FVector3 rightBorderPos(rightBorderX, borderY, startPosition.z + row * spacing);
        m_entityFactory->CreateStaticBox(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f), FVector3(0.0f, 0.0f, 0.0f),
                                         FVector3(1.0f, 0.0f, 0.0f)); // Red color
        m_entityFactory->CreateInvisibleBoxCollider(rightBorderPos, FVector3(3.0f, borderHeight, 3.0f),
                                                    FVector3(0.0f, 0.0f, 0.0f), boundaryGridElasticity,
                                                    boundaryGridFriction);
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
            float friction = iceGridFriction;

            Entity box = m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f),
                                                          FVector3(0.0f, 0.0f, 0.0f), currentColor);

            // Removed individual ColliderComponent and BoxBoundsComponent
        }
    }

    FVector3 iceColliderPosition =
        FVector3(startPosition2.x + ((gridColumns - 1) * spacing) / 2.0f, // X position remains the same
                 startPosition2.y,                                        // Y position remains the same
                 startPosition2.z + ((gridRows - 1) * spacing) / 2.0f     // Adjusted Z position
        );

    FVector3 iceColliderExtents = FVector3((gridColumns * spacing) / 2.0f, // X extents remain the same
                                           2.0f,                           // Y extents remain the same
                                           ((gridRows)*spacing) / 2.0f     // Adjusted Z extents
    );

    m_entityFactory->CreateInvisibleBoxCollider(iceColliderPosition, iceColliderExtents, FVector3(0.0f, 0.0f, 0.0f),
                                                iceGridElasticity, iceGridFriction);

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
                                                    boundaryGridFriction);

        // Right Border
        FVector3 rightBorderPos2(rightBorderX2, borderY2, startPosition2.z + row * spacing);
        m_entityFactory->CreateStaticBox(rightBorderPos2, FVector3(3.0f, borderHeight, 3.0f),
                                         FVector3(0.0f, 0.0f, 0.0f), FVector3(1.0f, 0.0f, 0.0f)); // Red color
        m_entityFactory->CreateInvisibleBoxCollider(rightBorderPos2, FVector3(3.0f, borderHeight, 3.0f),
                                                    FVector3(0.0f, 0.0f, 0.0f), boundaryGridElasticity,
                                                    boundaryGridFriction);
    }

    // Additional Entities
    // Ramp
    m_entityFactory->CreateStaticBox(FVector3(0.0f, 2.5f, -38.0f), FVector3(18.5f, 1.0f, 6.0f),
                                     FVector3(0.2f, 0.0f, 0.0f), FVector3(0.5f, 0.5f, 0.5f));
    m_entityFactory->CreateInvisibleBoxCollider(FVector3(0.0f, 2.5f, -38.0f), FVector3(18.5f, 1.0f, 6.0f),
                                                FVector3(-0.2f, 0.0f, 0.0f), 0.2f,
                                                0.5f); // Corrected friction parameter

     m_entityFactory->CreateStaticBox(FVector3(-8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                      FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
     m_entityFactory->CreateInvisibleBoxCollider(FVector3(-8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                                 FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity,
                                                 bouncyGridFriction);

     m_entityFactory->CreateStaticBox(FVector3(8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                      FVector3(0.0f, 0.0f, 0.0f), bouncyBoxColor);
     m_entityFactory->CreateInvisibleBoxCollider(FVector3(8.0f, 10.0f, -135.0f), FVector3(3.0f, 2.0f, 3.0f),
                                                 FVector3(0.0f, 0.0f, 0.0f), bouncyGridElasticity,
                                                 bouncyGridFriction);

    // Adjusted Ramp Positioning
    FVector3 rampStartPosition(0, 5, -97.5);

    // Create Ramp as a Single Sloped Piece
    m_entityFactory->CreateStaticBox(rampStartPosition, FVector3(18.5f, 1.0f, 6.0f), // Dimensions
                                     FVector3(0.2f, 0.0f, 0.0f),                     // Rotation
                                     FVector3(0.5f, 0.5f, 0.5f)                      // Color
    );

    // Create Collider for Ramp
    m_entityFactory->CreateInvisibleBoxCollider(rampStartPosition,
                                                FVector3(18.5f, 1.0f, 6.0f), // Extents matching the ramp dimensions
                                                FVector3(-0.2f, 0.0f, 0.0f), // Rotation
                                                0.2f, 0.5f);                 // Elasticity and friction

    // Define the start position for the sand rows at the top of the second ramp
    FVector3 sandStartPosition(20.0f, 5.25f, -145.0f); // Adjust Y and Z as necessary

    // --------------------
    // 6. Create Central Grass Grid with Sand Borders
    // --------------------

    // Configuration Parameters for Central Grid
    const int centralGrassRows = 8;
    const int centralGrassColumns = 8;
    const int sandBorderWidth = 3;      // Three-wide sand border
    const float grassSpacing = spacing; // Using existing spacing variable

    // Starting position for the central grass grid
    FVector3 centralGrassStartPosition = sandStartPosition; // Positioning relative to existing sand rows
    centralGrassStartPosition.x += -((centralGrassColumns + 2 * sandBorderWidth) * grassSpacing) / 2.0f;
    centralGrassStartPosition.z += -((centralGrassRows + 2 * sandBorderWidth) * grassSpacing) / 2.0f;

    // Colors for the grass grid
    FVector3 centralFirstGrassColor = firstGrassColor;
    FVector3 centralSecondGrassColor = secondGrassColor;

    // Create Central Grass Grid
    for (int row = 0; row < centralGrassRows; ++row)
    {
        for (int col = 0; col < centralGrassColumns; ++col)
        {
            FVector3 position = centralGrassStartPosition;
            position.x += col * grassSpacing;
            position.z += row * grassSpacing;

            FVector3 currentColor = (row + col) % 2 == 0 ? centralFirstGrassColor : centralSecondGrassColor;

            Entity grassBox = m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f),
                                                               FVector3(0.0f, 0.0f, 0.0f), currentColor);
            // Removed individual ColliderComponent and BoxBoundsComponent
        }
    }

    // Calculate Central Grass Collider Position and Extents
    FVector3 centralGrassColliderPosition(
        centralGrassStartPosition.x + ((centralGrassColumns - 1) * grassSpacing) / 2.0f, centralGrassStartPosition.y,
        centralGrassStartPosition.z + ((centralGrassRows - 1) * grassSpacing) / 2.0f);

    FVector3 centralGrassColliderExtents((centralGrassColumns * grassSpacing) / 2.0f, // Half width
                                         2.0f,                                        // Y extent (matches block height)
                                         (centralGrassRows * grassSpacing) / 2.0f     // Half depth
    );

    // Create Central Grass Collider
    m_entityFactory->CreateInvisibleBoxCollider(centralGrassColliderPosition, centralGrassColliderExtents,
                                                FVector3(0.0f, 0.0f, 0.0f), grassGridElasticity, grassGridFriction);

    // Create Sand Borders Around Central Grass Grid

    // Total dimensions including sand borders
    const int totalSandRows = centralGrassRows + 2 * sandBorderWidth;
    const int totalSandColumns = centralGrassColumns + 2 * sandBorderWidth;

    // Starting position for sand borders
    FVector3 sandBordersStartPosition = centralGrassStartPosition;
    sandBordersStartPosition.x -= sandBorderWidth * grassSpacing;
    sandBordersStartPosition.z -= sandBorderWidth * grassSpacing;

    // Create Sand Border Boxes
    for (int row = 0; row < totalSandRows; ++row)
    {
        for (int col = 0; col < totalSandColumns; ++col)
        {
            // Check if the current position is within the sand border
            if (row < sandBorderWidth || row >= (totalSandRows - sandBorderWidth) || col < sandBorderWidth ||
                col >= (totalSandColumns - sandBorderWidth))
            {
                FVector3 position = sandBordersStartPosition;
                position.x += col * grassSpacing;
                position.z += row * grassSpacing;

                // Create the sand box
                Entity sandBorderBox = m_entityFactory->CreateStaticBox(position, FVector3(3.0f, 2.0f, 3.0f),
                                                                        FVector3(0.0f, 0.0f, 0.0f), sandBoxColor);
            }
        }
    }

    // Create Separate Colliders for Each Sand Border Side

    // Left Sand Border Collider
    FVector3 leftSandColliderPosition(sandBordersStartPosition.x + (sandBorderWidth * grassSpacing) / 2.0f,
                                      sandBordersStartPosition.y,
                                      sandBordersStartPosition.z + ((totalSandRows - 1) * grassSpacing) / 2.0f);

    FVector3 leftSandColliderExtents((sandBorderWidth * grassSpacing) / 2.0f, // Half width
                                     2.0f,                                    // Y extent
                                     (totalSandRows * grassSpacing) / 2.0f    // Half depth
    );

    m_entityFactory->CreateInvisibleBoxCollider(leftSandColliderPosition, leftSandColliderExtents,
                                                FVector3(0.0f, 0.0f, 0.0f), sandGridElasticity, sandGridFriction);

    // Right Sand Border Collider
    FVector3 rightSandColliderPosition(
        sandBordersStartPosition.x + ((centralGrassColumns + sandBorderWidth) * grassSpacing) -
            (sandBorderWidth * grassSpacing) / 2.0f,
        sandBordersStartPosition.y, sandBordersStartPosition.z + ((totalSandRows - 1) * grassSpacing) / 2.0f);

    FVector3 rightSandColliderExtents((sandBorderWidth * grassSpacing) / 2.0f, // Half width
                                      2.0f,                                    // Y extent
                                      (totalSandRows * grassSpacing) / 2.0f    // Half depth
    );

    m_entityFactory->CreateInvisibleBoxCollider(rightSandColliderPosition, rightSandColliderExtents,
                                                FVector3(0.0f, 0.0f, 0.0f), sandGridElasticity, sandGridFriction);

    // Front Sand Border Collider
    FVector3 frontSandColliderPosition(sandBordersStartPosition.x + ((totalSandColumns)*grassSpacing) / 2.0f,
                                       sandBordersStartPosition.y,
                                       sandBordersStartPosition.z + (sandBorderWidth * grassSpacing) / 2.0f);

    FVector3 frontSandColliderExtents((totalSandColumns * grassSpacing) / 2.0f, // Half width
                                      2.0f,                                     // Y extent
                                      (sandBorderWidth * grassSpacing) / 2.0f   // Half depth
    );

    m_entityFactory->CreateInvisibleBoxCollider(frontSandColliderPosition, frontSandColliderExtents,
                                                FVector3(0.0f, 0.0f, 0.0f), sandGridElasticity, sandGridFriction);

    // Back Sand Border Collider
    FVector3 backSandColliderPosition(
        sandBordersStartPosition.x + ((totalSandColumns)*grassSpacing) / 2.0f, sandBordersStartPosition.y,
        sandBordersStartPosition.z + ((centralGrassRows + sandBorderWidth) * grassSpacing) -
            (sandBorderWidth * grassSpacing) / 2.0f);

    FVector3 backSandColliderExtents((totalSandColumns * grassSpacing) / 2.0f, // Half width
                                     2.0f,                                     // Y extent
                                     (sandBorderWidth * grassSpacing) / 2.0f   // Half depth
    );

    m_entityFactory->CreateInvisibleBoxCollider(backSandColliderPosition, backSandColliderExtents,
                                                FVector3(0.0f, 0.0f, 0.0f), sandGridElasticity, sandGridFriction);
}

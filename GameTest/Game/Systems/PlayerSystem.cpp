#include "stdafx.h"
#include "PlayerSystem.h"
#include <App/app.h>
#include <Game/Core/Components.h>
#include <Game/Core/Entity.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>
#include <Game/Math/MathUtil.h>

PlayerSystem::PlayerSystem(Registry *registry, EventManager *eventManager, PlayerManager *playerManager)
    : m_arrowView(registry)
{
    m_registry = registry;
    m_eventManager = eventManager;
    m_playerManager = playerManager;
}

void PlayerSystem::Init()
{
    
}

void PlayerSystem::Update(float dt)
{
    const float ROTATION_SPEED = 90.0f;
    const float OFFSET_DISTANCE = 5.0f;
    const float MAX_FORCE_SCALE = 500000.0f;
    const float VELOCITY_THRESHOLD = 0.25f;
    const float FORCE_INCREMENT = 10.0f;
    const float POSITION_RESET_Y = -5.0f;

    // Create a view for players with necessary components
    auto playerView = m_registry->CreateView<PlayerComponent, RigidBodyComponent, TransformComponent>();

    // Retrieve input states
    bool PITCHUP = App::IsKeyPressed('W'); // Pitch Up
    bool PITCHDOWN = App::IsKeyPressed('S'); // Pitch Down
    bool YAWLEFT = App::IsKeyPressed('A'); // Yaw Left
    bool YAWRIGHT = App::IsKeyPressed('D'); // Yaw Right
    bool SPACE = App::IsKeyPressed(VK_SPACE);
    bool TEST = App::IsKeyPressed('T');
    bool MOVEUP = App::IsKeyPressed('I');
    bool MOVEDOWN = App::IsKeyPressed('K');
    bool MOVERIGHT = App::IsKeyPressed('L');
    bool MOVELEFT = App::IsKeyPressed('J');
    bool MOVEUPWARD = App::IsKeyPressed('U');   // Move Up
    bool MOVEDOWNWARD = App::IsKeyPressed('O'); // Move Down
    float moveSpeed = 10.0f;
    std::string message = "MainMenu";
    if (TEST)
    {
        m_eventManager->Notify("SceneChange", "MainMenu");
    }

    // Update arrow view
    m_arrowView.Update();

    // Determine current player
    bool isPlayer1Turn = m_playerManager->IsPlayer1Turn();
    Entity currentPlayer = isPlayer1Turn ? m_playerManager->GetPlayer1() : m_playerManager->GetPlayer2();

    // Check if current player has a RigidBodyComponent
    if (!m_registry->HasComponent<RigidBodyComponent>(currentPlayer))
    {
        return;
    }

    RigidBodyComponent &ballRigidbody = m_registry->GetComponent<RigidBodyComponent>(currentPlayer);
    float ballSpeed = ballRigidbody.linearVelocity.Length();

    // Handle shot fired state
    if (shotFired)
    {
        if (ballSpeed < VELOCITY_THRESHOLD)
        {
            shotFired = false;
            m_playerManager->SwapTurn();
        }

        // Set all arrow layers to -1
        m_arrowView.ForEach(
            [&](Entity, ArrowComponent &, ModelComponent &model, TransformComponent &) { model.layer = -1; });
        // Do not return here to allow reset logic to execute
    }

    // Update arrow layer based on ball speed
    int arrowLayer = (ballSpeed < VELOCITY_THRESHOLD) ? 1 : -1;
    m_arrowView.ForEach(
        [&](Entity, ArrowComponent &, ModelComponent &model, TransformComponent &) { model.layer = arrowLayer; });

    // Update arrows related to the current player
    m_arrowView.ForEach(
        [&](Entity entity, ArrowComponent &arrow, ModelComponent &model, TransformComponent &transform) {
            if (arrow.ball != currentPlayer || shotFired)
            {
                model.layer = -1;
                return;
            }

            model.layer = 1;

            // Get player transform and rigidbody
            TransformComponent &playerTransform = m_registry->GetComponent<TransformComponent>(arrow.ball);
            RigidBodyComponent &rigidbody = m_registry->GetComponent<RigidBodyComponent>(arrow.ball);

            // Calculate rotation deltas
            float deltaYaw = 0.0f;
            if (YAWLEFT)
                deltaYaw += ROTATION_SPEED * dt; // Yaw Left
            if (YAWRIGHT)
                deltaYaw -= ROTATION_SPEED * dt; // Yaw Right

            float deltaPitch = 0.0f;
            if (PITCHUP)
                deltaPitch += ROTATION_SPEED * dt; // Pitch Up
            if (PITCHDOWN)
                deltaPitch -= ROTATION_SPEED * dt; // Pitch Down

            float deltaYawRadians = MathUtil::DegreesToRadians(deltaYaw);
            float deltaPitchRadians = MathUtil::DegreesToRadians(deltaPitch);

            FVector3 forward = (transform.Position - playerTransform.Position).Normalize();
            FVector3 right = forward.Cross(FVector3{0.0f, 1.0f, 0.0f}).Normalize();
            FVector3 localUp = right.Cross(forward).Normalize();                    

            // Create rotation quaternions based on local axes
            Quaternion deltaYawRotation = Quaternion::FromAxisAngle(FVector3(0.0f,1.0f,0.0f), deltaYawRadians);
            Quaternion deltaPitchRotation = Quaternion::FromAxisAngle(right, deltaPitchRadians);                

            // Update relative offset
            FVector3 relativeOffset = transform.Position - playerTransform.Position;
            relativeOffset = deltaYawRotation.RotateVector3(relativeOffset);
            relativeOffset = deltaPitchRotation.RotateVector3(relativeOffset);
            relativeOffset = relativeOffset.Normalize() * OFFSET_DISTANCE;
            transform.Position = playerTransform.Position + relativeOffset;

            // Update rotation to look in the direction of the arrow
            FVector3 direction = relativeOffset.Normalize();
            //transform.Rotation = Quaternion::LookAtPlusZ(direction, FVector3{0.0f, .0f, 0.0f});

            if (!SPACE && previousSpace)
            {
                m_playerManager->IncrementCurrentPlayerScore();

                rigidbody.force += direction * 2.0f * forceScale;

                const float angularVelocityMagnitude = 5.0f; 

                FVector3 direction = relativeOffset.Normalize();
                FVector3 forwardSpinAxis = FVector3{0.0f, 1.0f, 0.0f}.Cross(direction).Normalize();

                if (forwardSpinAxis.Length() < 0.01f)
                {
                    forwardSpinAxis = FVector3{1.0f, 0.0f, 0.0f}; // Use a fallback axis for stability
                }

                // Apply angular velocity for top spin
                rigidbody.angularVelocity = forwardSpinAxis * angularVelocityMagnitude;


                App::PlaySoundW("assets/GolfHit.wav");
                shotFired = true;
            }


        });

    // Update previousSpace state and forceScale
    if (SPACE)
    {
        forceScale += FORCE_INCREMENT;
        if (forceScale > MAX_FORCE_SCALE)
        {
            forceScale = MAX_FORCE_SCALE;
        }
    }
    else
    {
        forceScale = 0.0f;
    }
    previousSpace = SPACE;

    // Update force scale text if applicable
    if (m_registry->HasComponent<TextComponent>(m_scaleTextEntity))
    {
        TextComponent &text = m_registry->GetComponent<TextComponent>(m_scaleTextEntity);
        text.text = "Force: " + std::to_string(static_cast<int>(forceScale));
    }
    // Handle player movement with IJKL keys for testing
    if (MOVEUP)
    {
        ballRigidbody.force += FVector3(0.0f, 0.0f, moveSpeed); // Move Forward
    }
    if (MOVEDOWN)
    {
        ballRigidbody.force += FVector3(0.0f, 0.0f, -moveSpeed); // Move Backward
    }
    if (MOVERIGHT)
    {
        ballRigidbody.force += FVector3(moveSpeed, 0.0f, 0.0f); // Move Right
    }
    if (MOVELEFT)
    {
        ballRigidbody.force += FVector3(-moveSpeed, 0.0f, 0.0f); // Move Left
    }
    if (MOVEUPWARD)
    {
        ballRigidbody.force += FVector3(0.0f, moveSpeed, 0.0f); // Move Up
    }
    if (MOVEDOWNWARD)
    {
        ballRigidbody.force += FVector3(0.0f, -moveSpeed, 0.0f); // Move Down
    }
   
    // Handle player movement and rotation
    playerView.ForEach(
        [&](Entity entity, PlayerComponent &player, RigidBodyComponent &rigidbody, TransformComponent &transform) {
            // Reset conditions
            if (transform.Position.y <= POSITION_RESET_Y)
            {
                rigidbody = RigidBodyComponent(); // Reset to default state
                rigidbody.linearVelocity = FVector3{0.0f, 0.0f, 0.0f};
                rigidbody.angularVelocity = FVector3{0.0f, 0.0f, 0.0f};
                rigidbody.force = FVector3{0.0f, 0.0f, 0.0f};
                rigidbody.torque = FVector3{0.0f, 0.0f, 0.0f};
                transform.Position = player.spawnPoint;
            }

        });
}


void PlayerSystem::OnCollision(unsigned int ID1, unsigned int ID2)
{
    Entity player1ID = m_playerManager->GetPlayer1();
    Entity player2ID = m_playerManager->GetPlayer2();
    Entity holeID = m_playerManager->GetHole();

    // Check if any player collides with hole
    bool player1Collides = (ID1 == player1ID && ID2 == holeID) || (ID1 == holeID && ID2 == player1ID);
    bool player2Collides = (ID1 == player2ID && ID2 == holeID) || (ID1 == holeID && ID2 == player2ID);

    if (player1Collides)
    {
        App::PlaySoundW("assets/GolfHole.wav");
        m_eventManager->Notify("EmitParticles", FVector2{0.5f, 0.5f}, 100);

        m_playerManager->SetPlayer1Complete(true);
        m_registry->GetComponent<ModelComponent>(player1ID).layer = -1;
        m_registry->RemoveComponent<ColliderComponent>(player1ID);

        m_playerManager->SwapTurn();
    }
    else if (player2Collides)
    {
        App::PlaySoundW("assets/GolfHole.wav");
        m_eventManager->Notify("EmitParticles", FVector2{0.5f, 0.5f}, 100);

        m_playerManager->SetPlayer2Complete(true);
        m_registry->GetComponent<ModelComponent>(player2ID).layer = -1;
        m_registry->RemoveComponent<ColliderComponent>(player2ID);

        m_playerManager->SwapTurn();
    }

}

void PlayerSystem::SetScaleTextEntity(Entity scaleTextEntity)
{
    m_scaleTextEntity = scaleTextEntity;
}

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
    bool UP = App::IsKeyPressed('Q');
    bool LEFT = App::IsKeyPressed('A');
    bool DOWN = App::IsKeyPressed('E');
    bool RIGHT = App::IsKeyPressed('D');
    bool FORWARD = App::IsKeyPressed('W');
    bool BACKWARD = App::IsKeyPressed('S');
    bool R = App::IsKeyPressed('R');
    bool F = App::IsKeyPressed('F');
    bool V = App::IsKeyPressed('V');
    bool I = App::IsKeyPressed('I'); // Pitch Up
    bool K = App::IsKeyPressed('K'); // Pitch Down
    bool J = App::IsKeyPressed('J'); // Yaw Left
    bool L = App::IsKeyPressed('L'); // Yaw Right
    bool SPACE = App::IsKeyPressed(VK_SPACE);

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
            if (J)
                deltaYaw += ROTATION_SPEED * dt; // Yaw Left
            if (L)
                deltaYaw -= ROTATION_SPEED * dt; // Yaw Right

            float deltaPitch = 0.0f;
            if (I)
                deltaPitch += ROTATION_SPEED * dt; // Pitch Up
            if (K)
                deltaPitch -= ROTATION_SPEED * dt; // Pitch Down

            // Convert degrees to radians
            float deltaYawRadians = MathUtil::DegreesToRadians(deltaYaw);
            float deltaPitchRadians = MathUtil::DegreesToRadians(deltaPitch);

            // Create rotation quaternions
            Quaternion deltaYawRotation(FVector3{0.0f, deltaYawRadians, 0.0f});
            Quaternion deltaPitchRotation(FVector3{deltaPitchRadians, 0.0f, 0.0f});

            // Update relative offset
            FVector3 relativeOffset = transform.Position - playerTransform.Position;
            relativeOffset = deltaYawRotation * relativeOffset;
            relativeOffset = deltaPitchRotation * relativeOffset;
            relativeOffset = relativeOffset.Normalize() * OFFSET_DISTANCE;
            transform.Position = playerTransform.Position + relativeOffset;

            // Update rotation to look in the direction of the arrow
            FVector3 direction = relativeOffset.Normalize();
            transform.Rotation =
                Quaternion::Slerp(transform.Rotation, Quaternion::LookAtPlusZ(direction, FVector3{0, 1, 0}), 0.1f);

            // Handle shooting when SPACE is released
            if (!SPACE && previousSpace)
            {
                m_playerManager->IncrementCurrentPlayerScore();
                rigidbody.force += direction * 2.0f * forceScale;
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

            // Apply movement forces only if not resetting
            if (!(transform.Position.y <= POSITION_RESET_Y))
            {
                if (UP)
                    rigidbody.force += FVector3{0.0f, 1.0f, 0.0f};
                if (DOWN)
                    rigidbody.force -= FVector3{0.0f, 1.0f, 0.0f};
                if (LEFT)
                    rigidbody.force -= FVector3{1.0f, 0.0f, 0.0f};
                if (RIGHT)
                    rigidbody.force += FVector3{1.0f, 0.0f, 0.0f};
                if (FORWARD)
                    rigidbody.force -= FVector3{0.0f, 0.0f, 1.0f};
                if (BACKWARD)
                    rigidbody.force += FVector3{0.0f, 0.0f, 1.0f};

                // Apply rotation torques
                if (R)
                    rigidbody.torque += FVector3{0.0f, 0.0f, 1.0f} * ROTATION_SPEED;
                if (F)
                    rigidbody.torque += FVector3{0.0f, 1.0f, 0.0f} * ROTATION_SPEED;
                if (V)
                    rigidbody.torque += FVector3{1.0f, 0.0f, 0.0f} * ROTATION_SPEED;
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

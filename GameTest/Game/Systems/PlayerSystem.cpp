#include "stdafx.h"
#include "PlayerSystem.h"
#include <App/app.h>
#include <Game/Core/Components.h>
#include <Game/Core/Entity.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>
#include <Game/Math/MathUtil.h>

PlayerSystem::PlayerSystem(Registry *registry, EventManager *eventManager, PlayerManager *playerManager, Camera* camera)
    : m_arrowView(registry), m_camera(camera)
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
            [&](Entity, ArrowComponent &, ModelComponent &model, TransformComponent &) { model.enabled = false; });
        // Do not return here to allow reset logic to execute
    }

    // Update arrow layer based on ball speed
    bool  enabled = (ballSpeed < VELOCITY_THRESHOLD) ? true : false;
    m_arrowView.ForEach(
        [&](Entity, ArrowComponent &, ModelComponent &model, TransformComponent &) { model.enabled = enabled; });

    // Update arrows related to the current player
    m_arrowView.ForEach(
        [&](Entity entity, ArrowComponent &arrow, ModelComponent &model, TransformComponent &transform) {
            if (arrow.ball != currentPlayer)
            {
                model.enabled = false;
                return;
            }

            
            

            // Get player transform and rigidbody
            TransformComponent &playerTransform = m_registry->GetComponent<TransformComponent>(arrow.ball);
            RigidBodyComponent &rigidbody = m_registry->GetComponent<RigidBodyComponent>(arrow.ball);
            if (shotFired)
            {
                // Lerp camera towards ball's position based on shot direction
                CameraFollowComponent &cameraFollow = m_registry->GetComponent<CameraFollowComponent>(arrow.ball);

                // Desired camera position is behind the shot direction
                FVector3 desiredCameraPos = playerTransform.position - m_shotDirection * cameraFollow.offset.Length() +
                                            FVector3(0, cameraFollow.offset.y, 0);

                // Smoothly interpolate camera position
                FVector3 currentCameraPos = m_camera->GetPosition();
                FVector3 newCameraPos = MathUtil::Lerp(currentCameraPos, desiredCameraPos, cameraFollow.smoothSpeed * dt);
                //m_camera->SetPosition(newCameraPos);


                return;
            }
            model.enabled = true;
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

            // Update cumulative yaw and pitch angles
            totalYaw += deltaYaw;
            totalPitch += deltaPitch;

            // Clamp pitch to prevent flipping
            const float MAX_PITCH_ANGLE = 89.0f; // To avoid gimbal lock
            if (totalPitch > MAX_PITCH_ANGLE)
                totalPitch = MAX_PITCH_ANGLE;
            if (totalPitch < -MAX_PITCH_ANGLE)
                totalPitch = -MAX_PITCH_ANGLE;

            float deltaYawRadians = MathUtil::DegreesToRadians(deltaYaw);
            float deltaPitchRadians = MathUtil::DegreesToRadians(deltaPitch);                   

            Quaternion deltaYawRotation = Quaternion::FromAxisAngle(FVector3(0.0f,1.0f,0.0f), deltaYawRadians);
            Quaternion deltaPitchRotation = Quaternion::FromAxisAngle(FVector3(1.0f,0.0,0.0f), deltaPitchRadians);                

            FVector3 relativeOffset = transform.position - playerTransform.position;
            relativeOffset = deltaYawRotation.RotateVector3(relativeOffset);
            relativeOffset = deltaPitchRotation.RotateVector3(relativeOffset);
            relativeOffset = relativeOffset.Normalize() * OFFSET_DISTANCE;
            FVector3 newCameraPos = transform.position - (relativeOffset*12);
            newCameraPos.y = playerTransform.position.y;
            newCameraPos += FVector3(0, 8, 0);
            m_camera->SetPosition(newCameraPos);
            m_camera->AddYaw(deltaYaw);

            float yawRadians = MathUtil::DegreesToRadians(totalYaw);
            float pitchRadians = MathUtil::DegreesToRadians(totalPitch);

            float cosPitch = cosf(pitchRadians);
            FVector3 direction;
            direction.x = sinf(yawRadians) * cosPitch;
            direction.y = sinf(pitchRadians);
            direction.z = cosf(yawRadians) * cosPitch;
            direction = direction.Normalize();
            //direction.z = -direction.z;

            transform.position = playerTransform.position +direction * OFFSET_DISTANCE;
            transform.rotation = Quaternion::LookAt(direction, FVector3{0.0f, -1.0f, 0.0f});
            if (!SPACE && previousSpace)
            {
                m_shotDirection = direction;
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
            if (transform.position.y <= POSITION_RESET_Y)
            {
                rigidbody = RigidBodyComponent(); // Reset to default state
                rigidbody.linearVelocity = FVector3{0.0f, 0.0f, 0.0f};
                rigidbody.angularVelocity = FVector3{0.0f, 0.0f, 0.0f};
                rigidbody.force = FVector3{0.0f, 0.0f, 0.0f};
                rigidbody.torque = FVector3{0.0f, 0.0f, 0.0f};
                transform.position = player.spawnPoint;
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
        m_eventManager->Notify("EmitParticles", FVector2(0.0, 0.0), 100, FVector3(0.5f, 0.0f, 1.0f));

        m_playerManager->SetPlayer1Complete(true);
        m_registry->GetComponent<ModelComponent>(player1ID).enabled = false;
        m_registry->RemoveComponent<ColliderComponent>(player1ID);

        m_playerManager->SwapTurn();
    }
    else if (player2Collides)
    {
        App::PlaySoundW("assets/GolfHole.wav");
        m_eventManager->Notify("EmitParticles", FVector2(0.0f, 0.0f), 100,FVector3 (1.0, 0.0, 0.5));

        m_playerManager->SetPlayer2Complete(true);
        m_registry->GetComponent<ModelComponent>(player2ID).enabled = false;
        m_registry->RemoveComponent<ColliderComponent>(player2ID);

        m_playerManager->SwapTurn();
    }
    if (m_playerManager->IsPlayer1Complete() && m_playerManager->IsPlayer2Complete())
    {
        m_eventManager->Notify("SceneChange", "Results");
    }

}

void PlayerSystem::SetScaleTextEntity(Entity scaleTextEntity)
{
    m_scaleTextEntity = scaleTextEntity;
}

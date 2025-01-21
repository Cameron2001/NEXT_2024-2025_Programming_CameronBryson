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
    : m_camera(camera), m_arrowView(registry), m_scaleTextEntity(0)
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
    constexpr float ROTATION_SPEED = 90.0f;
    constexpr float ARROW_DISTANCE = 8.0f;   
    constexpr float CAMERA_DISTANCE = 45.0f; 
    constexpr float CAMERA_HEIGHT = 5.0f;    
    constexpr float MAX_FORCE_SCALE = 500000.0f;
    constexpr float FORCE_INCREMENT = 10.0f;
    constexpr float VELOCITY_THRESHOLD = 2.0f;
    constexpr float POSITION_RESET_Y = -3.0f;

    auto playerView = m_registry->CreateView<PlayerComponent, RigidBodyComponent, TransformComponent>();

    const bool PITCHUP = App::IsKeyPressed('S');
    const bool PITCHDOWN = App::IsKeyPressed('W');
    const bool YAWLEFT = App::IsKeyPressed('A');
    const bool YAWRIGHT = App::IsKeyPressed('D');
    const bool SPACE = App::IsKeyPressed(VK_SPACE);
   

    m_arrowView.Update();

    // Which player's turn is it?
    bool isPlayer1Turn = m_playerManager->IsPlayer1Turn();
    Entity currentPlayer = isPlayer1Turn ? m_playerManager->GetPlayer1() : m_playerManager->GetPlayer2();

    if (!m_registry->HasComponent<RigidBodyComponent>(currentPlayer))
    {
        return;
    }

    RigidBodyComponent &ballRb = m_registry->GetComponent<RigidBodyComponent>(currentPlayer);
    float ballSpeed = ballRb.linearVelocity.Length();

    if (shotFired)
    {
        if (ballSpeed < VELOCITY_THRESHOLD)
        {
            shotFired = false;
            m_playerManager->SwapTurn();
        }
        m_arrowView.ForEach(
            [&](Entity, ArrowComponent &, ModelComponent &model, TransformComponent &) { model.enabled = false; });
    }

    bool showArrow = (ballSpeed < VELOCITY_THRESHOLD);
    m_arrowView.ForEach(
        [&](Entity, ArrowComponent &, ModelComponent &model, TransformComponent &) 
        {
            model.enabled = showArrow;
        });
    if (showArrow)
    {
        ballRb.angularVelocity = FVector3(0, 0, 0);
        ballRb.linearVelocity = FVector3(0, 0, 0);
        ballRb.force = FVector3(0, 0, 0);
        ballRb.torque = FVector3(0, 0, 0);
    }

    m_arrowView.ForEach([&](Entity entity, const ArrowComponent &arrow, ModelComponent &model, TransformComponent &arrowTf) {
        if (arrow.ball != currentPlayer)
        {
            model.enabled = false;
            return;
        }
        if (shotFired)
        {
            return;
        }

        model.enabled = true;

        const TransformComponent &playerTf = m_registry->GetComponent<TransformComponent>(arrow.ball);
        RigidBodyComponent &rb = m_registry->GetComponent<RigidBodyComponent>(arrow.ball);

        float deltaYaw = 0.0f;
        if (YAWLEFT)
            deltaYaw += ROTATION_SPEED * dt;
        if (YAWRIGHT)
            deltaYaw -= ROTATION_SPEED * dt;

        float deltaPitch = 0.0f;
        if (PITCHUP)
            deltaPitch += ROTATION_SPEED * dt;
        if (PITCHDOWN)
            deltaPitch -= ROTATION_SPEED * dt;

        totalYaw += deltaYaw;
        totalPitch += deltaPitch;

        constexpr float MAX_PITCH_ANGLE = 60.0f;
        if (totalPitch > MAX_PITCH_ANGLE)
            totalPitch = MAX_PITCH_ANGLE;
        if (totalPitch < -MAX_PITCH_ANGLE)
            totalPitch = -MAX_PITCH_ANGLE;
        const float yawRad = MathUtil::DegreesToRadians(totalYaw);
        const float pitchRad = MathUtil::DegreesToRadians(totalPitch);

        Quaternion yawQ = Quaternion::FromAxisAngle(FVector3(0, 1, 0), yawRad);
        Quaternion pitchQ = Quaternion::FromAxisAngle(FVector3(1, 0, 0), pitchRad);

        arrowTf.rotation = yawQ * pitchQ;

        const FVector3 arrowForward = arrowTf.rotation.RotateVector3(FVector3(0, 0, 1)).Normalize();

        arrowTf.position = playerTf.position + arrowForward * ARROW_DISTANCE;


        const float cameraYawRad = yawRad;

        const Quaternion cameraYawQ = Quaternion::FromAxisAngle(FVector3(0, -1, 0), cameraYawRad);


        static const Quaternion flipY180 =
            Quaternion::FromAxisAngle(FVector3(0, 1, 0), MathUtil::DegreesToRadians(180.0f));


        const Quaternion finalCamOrientation = cameraYawQ * flipY180;


        m_camera->SetOrientation(finalCamOrientation);

        

        const FVector3 cameraPos = playerTf.position - (arrowForward * FVector3(1,0,1) * CAMERA_DISTANCE) + FVector3(0, CAMERA_HEIGHT, 0);

        m_camera->SetPosition(cameraPos);

        if (!SPACE && previousSpace)
        {
            m_shotDirection = arrowForward;
            rb.force += arrowForward * 2.0f * forceScale;

            constexpr float angularVelocityMagnitude = 5.0f;
            FVector3 spinAxis = FVector3(0, 1, 0).Cross(arrowForward).Normalize();
            if (spinAxis.Length() < 0.01f)
            {
                spinAxis = FVector3(1, 0, 0);
            }
            rb.angularVelocity = spinAxis * angularVelocityMagnitude;
            m_playerManager->IncrementCurrentPlayerScore();
            App::PlaySoundW("assets/GolfHit.wav");
            shotFired = true;
        }
    });

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

    if (m_registry->HasComponent<TextComponent>(m_scaleTextEntity))
    {
        TextComponent &text = m_registry->GetComponent<TextComponent>(m_scaleTextEntity);
        text.text = "Force: " + std::to_string(static_cast<int>(forceScale));
    }


    playerView.ForEach([&](Entity e, const PlayerComponent &player, RigidBodyComponent &rb, TransformComponent &tf) {
        if (tf.position.y <= POSITION_RESET_Y)
        {
            rb = RigidBodyComponent();
            tf.position = player.spawnPoint;
        }
    });
}

void PlayerSystem::OnCollision(const unsigned int ID1, const unsigned int ID2)
{
    const Entity player1ID = m_playerManager->GetPlayer1();
    const Entity player2ID = m_playerManager->GetPlayer2();
    const Entity holeID = m_playerManager->GetHole();

    // Check if any player collides with hole
    const bool player1Collides = (ID1 == player1ID && ID2 == holeID) || (ID1 == holeID && ID2 == player1ID);
    const bool player2Collides = (ID1 == player2ID && ID2 == holeID) || (ID1 == holeID && ID2 == player2ID);

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
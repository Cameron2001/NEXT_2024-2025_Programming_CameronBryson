#include "stdafx.h"
#include "ArrowSystem.h"
#include "Game/Math/MathUtil.h"
#include <algorithm>
#include <App/app.h>

ArrowSystem::ArrowSystem(Registry *registry, PlayerManager *playerManager)
    : m_registry(registry), m_arrowView(registry), m_playerManager(playerManager)
{
}

void ArrowSystem::SetScaleTextEntity(Entity scaleTextEntity)
{
    m_scaleTextEntity = scaleTextEntity;
}


void ArrowSystem::Update(float dt)
{
    bool I = App::IsKeyPressed('I'); // Pitch Up
    bool K = App::IsKeyPressed('K'); // Pitch Down
    bool J = App::IsKeyPressed('J'); // Yaw Left
    bool L = App::IsKeyPressed('L'); // Yaw Right
    bool SPACE = App::IsKeyPressed(VK_SPACE);

    m_arrowView.Update();

    constexpr float rotationSpeed = 90.0f;
    constexpr float offsetDistance = 5.0f;
    constexpr float maxForceScale = 500000.0f;

    // Determine the current player
    bool isPlayer1Turn = m_playerManager->IsPlayer1Turn();
    Entity currentPlayer = isPlayer1Turn ? m_playerManager->GetPlayer1() : m_playerManager->GetPlayer2();

    m_arrowView.ForEach(
        [this, dt, rotationSpeed, I, K, J, L, SPACE, offsetDistance, maxForceScale,
         currentPlayer](Entity entity, ArrowComponent &arrow, ModelComponent &model, TransformComponent &transform) {
            // Check if the arrow belongs to the current player
            if (arrow.ball != currentPlayer)
            {
                // Hide the arrow
                model.layer = -1;
                return;
            }

            model.layer = 1;

            TransformComponent &playerTransform = m_registry->GetComponent<TransformComponent>(arrow.ball);
            RigidBodyComponent &rigidbody = m_registry->GetComponent<RigidBodyComponent>(arrow.ball);

            float rotationDeltaYaw = 0.0f;
            float rotationDeltaPitch = 0.0f;

            if (J)
                rotationDeltaYaw += rotationSpeed * dt; // Rotate left
            if (L)
                rotationDeltaYaw -= rotationSpeed * dt; // Rotate right

            if (I)
                rotationDeltaPitch += rotationSpeed * dt; // Pitch up
            if (K)
                rotationDeltaPitch -= rotationSpeed * dt; // Pitch down

            float deltaYawRadians = MathUtil::DegreesToRadians(rotationDeltaYaw);
            float deltaPitchRadians = MathUtil::DegreesToRadians(rotationDeltaPitch);

            Quaternion deltaYawRotation(FVector3{0.0f, deltaYawRadians, 0.0f});
            Quaternion deltaPitchRotation(FVector3{deltaPitchRadians, 0.0f, 0.0f});

            FVector3 relativeOffset = transform.Position - playerTransform.Position;
            relativeOffset = deltaYawRotation * relativeOffset;
            relativeOffset = deltaPitchRotation * relativeOffset;

            relativeOffset = relativeOffset.Normalize() * offsetDistance;

            transform.Position = playerTransform.Position + relativeOffset;

            FVector3 direction = relativeOffset.Normalize();
            transform.Rotation =
                Quaternion::Slerp(transform.Rotation, Quaternion::LookAt(direction, FVector3{0, 1, 0}), 0.1f);

            FVector3 arrowDirection = direction;

            // Apply force when SPACE is released
            if (!SPACE && previousSpace)
            {
                rigidbody.force += arrowDirection * 2.0f * forceScale;
                App::PlaySoundW("assets/GolfHit.wav");
                m_playerManager->SwapTurn();
            }
        });

    previousSpace = SPACE;
    if (SPACE)
    {
        forceScale += 10.0f;
    }
    else
    {
        forceScale = 0.0f;
    }
    if (m_registry->HasComponent<TextComponent>(m_scaleTextEntity))
    {
        TextComponent &text = m_registry->GetComponent<TextComponent>(m_scaleTextEntity);
        text.text = "Force: " + std::to_string(static_cast<int>(forceScale));
    }
}

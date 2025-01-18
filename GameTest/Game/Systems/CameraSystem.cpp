#include "stdafx.h"
#include "CameraSystem.h"
#include "Game/Math/MathUtil.h"
#include <App/app.h>

CameraSystem::CameraSystem(Registry *registry, PlayerManager* playerManager, Camera *camera)
    : m_registry(registry), m_playerManager(playerManager),m_camera(camera), m_view(registry)
{
}

void CameraSystem::Update(float dt)
{
    bool ARROW_LEFT = App::IsKeyPressed(VK_LEFT);
    bool ARROW_RIGHT = App::IsKeyPressed(VK_RIGHT);
    bool ARROW_UP = App::IsKeyPressed(VK_UP);
    bool ARROW_DOWN = App::IsKeyPressed(VK_DOWN);

    constexpr float rotationSpeed = 1.0f;

    if (ARROW_LEFT)
    {
        Quaternion delta = Quaternion::Slerp(Quaternion(), Quaternion({0.0f, -rotationSpeed, 0.0f}), dt);
        m_camera->Rotate(delta);
    }
    if (ARROW_RIGHT)
    {
        Quaternion delta = Quaternion::Slerp(Quaternion(), Quaternion({0.0f, rotationSpeed, 0.0f}), dt);
        m_camera->Rotate(delta);
    }
    if (ARROW_UP)
    {
        Quaternion delta = Quaternion::Slerp(Quaternion(), Quaternion({-rotationSpeed, 0.0f, 0.0f}), dt);
        m_camera->Rotate(delta);
    }
    if (ARROW_DOWN)
    {
        Quaternion delta = Quaternion::Slerp(Quaternion(), Quaternion({rotationSpeed, 0.0f, 0.0f}), dt);
        m_camera->Rotate(delta);
    }

    m_view.Update();

    bool isPlayer1Turn = m_playerManager->IsPlayer1Turn();
    Entity currentPlayer = isPlayer1Turn ? m_playerManager->GetPlayer1() : m_playerManager->GetPlayer2();

    m_view.ForEach([&](Entity entity, CameraFollowComponent &cameraFollow, TransformComponent &transform) {
        if (entity == currentPlayer)
        {
            FVector3 targetPosition = transform.Position + cameraFollow.offset;
            FVector3 smoothedPosition =
                MathUtil::Lerp(m_camera->GetPosition(), targetPosition, cameraFollow.smoothSpeed * dt);
            m_camera->SetPosition(smoothedPosition);
        }
    });
}

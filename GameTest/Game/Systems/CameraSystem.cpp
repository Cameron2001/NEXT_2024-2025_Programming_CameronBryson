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

    constexpr float rotationSpeed = 45.0f; // degrees per second
    float moveSpeed = 50.0f;

    if (ARROW_LEFT)
    {
        // Rotate left by increasing yaw
        m_camera->AddYaw(rotationSpeed * dt);
        //m_camera->SetPosition(m_camera->GetPosition() - m_camera->GetRight() * moveSpeed * dt);
    }
    if (ARROW_RIGHT)
    {
        // Rotate right by decreasing yaw
        m_camera->AddYaw(-rotationSpeed * dt);
        //m_camera->SetPosition(m_camera->GetPosition() + m_camera->GetRight() * moveSpeed * dt);
    }
    if (ARROW_UP)
    {
        m_camera->AddPitch(-rotationSpeed * dt);
    }
    if (ARROW_DOWN)
    {
        // Look down by decreasing pitch
        m_camera->AddPitch(rotationSpeed * dt);
        //m_camera->SetPosition(m_camera->GetPosition() - FVector3(0, 1, 0) * moveSpeed * dt);
    }


    m_view.Update();

    bool isPlayer1Turn = m_playerManager->IsPlayer1Turn();
    Entity currentPlayer = isPlayer1Turn ? m_playerManager->GetPlayer1() : m_playerManager->GetPlayer2();

    m_view.ForEach([&](Entity entity, CameraFollowComponent &cameraFollow, TransformComponent &transform) {
        if (entity == currentPlayer)
        {
            FVector3 targetPosition = transform.position + cameraFollow.offset;
            FVector3 smoothedPosition =
                MathUtil::Lerp(m_camera->GetPosition(), targetPosition, cameraFollow.smoothSpeed * dt);
            //m_camera->SetPosition(smoothedPosition);
            //m_camera->LookAt(FVector3(transform.Position.x, transform.Position.y, transform.Position.z));
        }
    });
}

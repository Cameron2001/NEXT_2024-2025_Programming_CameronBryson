#include "stdafx.h"
#include "Camera.h"
#include "Game/Math/MathUtil.h"
#include "App/AppSettings.h" 
#include <cassert>
#include <cmath>

Camera::Camera(const FVector3 &position)
    : m_position(position), m_orientation(1.0f, 0.0f, 0.0f, 0.0f) 
      ,
      m_FOV(90.0f), m_zNear(0.1f), m_zFar(1000.0f), m_forward(0.0f, 0.0f, -1.0f) 
      ,
      m_up(0.0f, 1.0f, 0.0f) 
      ,
      m_right(1.0f, 0.0f, 0.0f) 
      ,
      m_worldUp(0.0f, 1.0f, 0.0f) 
      ,
      m_minPitch(-89.0f)
      ,
      m_maxPitch(89.0f) 
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}


void Camera::SetPosition(const FVector3 &position)
{
    m_position = position;
    UpdateViewMatrix();
}

void Camera::SetFOV(const float fov)
{
    m_FOV = fov;
    UpdateProjectionMatrix();
}

void Camera::SetOrientation(const Quaternion &orientation)
{
    m_orientation = orientation;
    UpdateViewMatrix();
}

void Camera::AddPitch(const float degrees)
{
    const float radians = MathUtil::DegreesToRadians(degrees);

    const FVector3 localX = m_orientation.RotateVector3(FVector3(1.0f, 0.0f, 0.0f)).Normalize();
    const Quaternion pitchQuat = Quaternion::FromAxisAngle(localX, radians);

    m_orientation = (pitchQuat * m_orientation).Normalize();

    ClampPitch();
    UpdateViewMatrix();
}

void Camera::AddYaw(const float degrees)
{
    float radians = MathUtil::DegreesToRadians(degrees);
    radians = -radians;

    const Quaternion yawQuat = Quaternion::FromAxisAngle(m_worldUp, radians);

    m_orientation = (yawQuat * m_orientation).Normalize();

    UpdateViewMatrix();
}

void Camera::ClampPitch()
{
    const float currentPitch = MathUtil::RadiansToDegrees(m_orientation.GetPitch());

    if (currentPitch > m_maxPitch)
    {
        const float excess = currentPitch - m_maxPitch;
        AddPitch(-excess);
    }
    else if (currentPitch < m_minPitch)
    {
        const float deficit = m_minPitch - currentPitch;
        AddPitch(deficit); 
    }
}

void Camera::UpdateViewMatrix()
{
    m_forward = m_orientation.RotateVector3(FVector3(0.0f, 0.0f, -1.0f)).Normalize();
    m_up = m_orientation.RotateVector3(FVector3(0.0f, 1.0f, 0.0f)).Normalize();
    m_right = m_orientation.RotateVector3(FVector3(1.0f, 0.0f, 0.0f)).Normalize();

    const FVector3 target = m_position + m_forward;

    m_viewMatrix = Matrix4::CreateViewMatrix(m_position, target, m_up);
}

void Camera::UpdateProjectionMatrix()
{
    constexpr float aspectRatio = static_cast<float>(APP_VIRTUAL_WIDTH) / static_cast<float>(APP_INIT_WINDOW_HEIGHT);
    m_projectionMatrix = Matrix4::CreatePerspectiveMatrix(m_FOV, aspectRatio, m_zNear, m_zFar);
}

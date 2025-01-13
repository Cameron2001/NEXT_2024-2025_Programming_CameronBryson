#include "stdafx.h"
#include "Camera.h"
#include "Engine/Math/MathUtil.h"
#include "App/SimpleSprite.h"
#include "App/AppSettings.h"

Camera::Camera(const FVector3 &position, const FVector3 &worldUp, const float yaw, const float pitch, const float FOV) :
    m_position(position), m_worldUp(worldUp), m_yaw(yaw), m_pitch(pitch), m_FOV(FOV), m_zNear(0.1f), m_zFar(100.0f)
{

    m_forward = FVector3(0.0f, 0.0f, -1.0f);
    m_up = FVector3(0.0f, 1.0f, 0.0f);
    m_right = FVector3(1.0f, 0.0f, 0.0f);
    UpdateAxes();
    UpdateProjectionMatrix();
}

void Camera::SetPosition(const FVector3 &position)
{
    m_position = position;
    UpdateViewMatrix();
}

void Camera::SetYaw(const float yaw)
{
    m_yaw = yaw;
    UpdateAxes();
}

void Camera::SetPitch(const float pitch)
{
    m_pitch = pitch;
    UpdateAxes();
}

void Camera::SetFOV(const float fov)
{
    m_FOV = fov;
    UpdateProjectionMatrix();
}

const FVector3 &Camera::GetPosition() const
{
    return m_position;
}

float Camera::GetYaw() const
{
    return m_yaw;
}

float Camera::GetPitch() const
{
    return m_pitch;
}

const Matrix4 &Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const Matrix4 &Camera::GetProjectionMatrix() const
{
    
    return m_projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
    m_viewMatrix = Matrix4::CreateViewMatrix(m_position, m_position + m_forward, m_up);
}

void Camera::UpdateProjectionMatrix()
{
    m_projectionMatrix = Matrix4::CreatePerspectiveMatrix(m_FOV, static_cast<float>(APP_VIRTUAL_WIDTH) / static_cast<float>(APP_INIT_WINDOW_HEIGHT),
                                                          m_zNear, m_zFar);
}

void Camera::UpdateAxes()
{
    const float radYaw = MathUtil::DegreesToRadians(m_yaw);
    const float radPitch = MathUtil::DegreesToRadians(m_pitch);

    m_forward.x = cosf(radYaw) * cosf(radPitch);
    m_forward.y = sinf(radPitch);
    m_forward.z = sinf(radYaw) * cosf(radPitch);
    m_forward = m_forward.Normalize();

    m_right = m_forward.Cross(m_worldUp).Normalize();
    m_up = m_right.Cross(m_forward).Normalize();
    UpdateViewMatrix();
}
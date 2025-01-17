#include "stdafx.h"
#include "Camera.h"
#include "Game/Math/MathUtil.h"
#include "App/SimpleSprite.h"
#include "App/AppSettings.h"

// Updated Constructor
Camera::Camera(const FVector3 &position, const FVector3 &worldUp, const Quaternion &orientation, float FOV)
    : m_position(position), m_worldUp(worldUp),
      m_orientation(orientation.Normalize()), // Ensure the quaternion is normalized
      m_FOV(FOV), m_zNear(0.1f), m_zFar(100.0f)
{
    UpdateAxes();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

// Set Position
void Camera::SetPosition(const FVector3 &position)
{
    m_position = position;
    UpdateViewMatrix();
}

// Set Orientation using Quaternion
void Camera::SetOrientation(const Quaternion &orientation)
{
    m_orientation = orientation.Normalize();
    UpdateAxes();
    UpdateViewMatrix();
}

// Rotate Camera using Quaternion
void Camera::Rotate(const Quaternion &delta)
{
    m_orientation = (delta * m_orientation).Normalize();
    UpdateAxes();
    UpdateViewMatrix();
}

// Set Field of View
void Camera::SetFOV(float fov)
{
    m_FOV = fov;
    UpdateProjectionMatrix();
}

// Getters
const FVector3 &Camera::GetPosition() const
{
    return m_position;
}

Quaternion Camera::GetOrientation() const
{
    return m_orientation;
}

float Camera::GetFOV() const
{
    return m_FOV;
}

const Matrix4 &Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const Matrix4 &Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

// Update the View Matrix using the current orientation and position
void Camera::UpdateViewMatrix()
{
    // The target is position + forward direction
    FVector3 target = m_position + m_forward;
    m_viewMatrix = Matrix4::CreateViewMatrix(m_position, target, m_up);
}

// Update the Projection Matrix based on FOV and aspect ratio
void Camera::UpdateProjectionMatrix()
{
    m_projectionMatrix = Matrix4::CreatePerspectiveMatrix(
        m_FOV, static_cast<float>(APP_VIRTUAL_WIDTH) / static_cast<float>(APP_INIT_WINDOW_HEIGHT), m_zNear, m_zFar);
}

// Update the Camera's Axes based on the current orientation
void Camera::UpdateAxes()
{
    // Rotate the default forward, up, and right vectors by the orientation quaternion
    m_forward = m_orientation * FVector3(0.0f, 0.0f, -1.0f); // Forward
    m_up = m_orientation * FVector3(0.0f, 1.0f, 0.0f);       // Up
    m_right = m_orientation * FVector3(1.0f, 0.0f, 0.0f);    // Right
}

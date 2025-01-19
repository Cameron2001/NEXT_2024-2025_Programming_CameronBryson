#include "stdafx.h"
#include "Camera.h"
#include "Game/Math/MathUtil.h"
#include "App/SimpleSprite.h"
#include "App/AppSettings.h"

// Updated Constructor
Camera::Camera(const FVector3 &position)
    : m_position(position), m_worldUp({0,1,0}), // Ensure the quaternion is normalized
      m_FOV(90), m_zNear(0.1f), m_zFar(1000.0f), m_pitch(0.0f), m_yaw(0.0f), m_minPitch(-80.0f), m_maxPitch(80.0f)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}


void Camera::LookAt(const FVector3 &target)
{
    m_followTarget = true;
    m_target = target;
    UpdateViewMatrix();
}


// Set Position
void Camera::SetPosition(const FVector3 &position)
{
    m_position = position;
    UpdateViewMatrix();
}

// Set Field of View
void Camera::SetFOV(float fov)
{
    m_FOV = fov;
    UpdateProjectionMatrix();
}

void Camera::AddPitch(float deltaPitch)
{
    m_followTarget = false;
    m_pitch += deltaPitch;

    // Clamp pitch to the range [m_minPitch, m_maxPitch]
    if (m_pitch > m_maxPitch)
        m_pitch = m_maxPitch;
    if (m_pitch < m_minPitch)
        m_pitch = m_minPitch;
    UpdateViewMatrix();

}

void Camera::AddYaw(float deltaYaw)
{
    m_followTarget = false;
    if (m_pitch > 90.0f && m_pitch < 270.0f|| (m_pitch < -90.0f && m_pitch > -270.0f))
    {
        m_yaw -= deltaYaw;
    }
    else
    {
        m_yaw += deltaYaw;
    }
    if (m_yaw > 180.0f)
        m_yaw -= 360.0f;
    else if (m_yaw < -180.0f)
        m_yaw += 360.0f;
    UpdateViewMatrix();

}

// Getters
const FVector3 &Camera::GetPosition() const
{
    return m_position;
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

FVector3 Camera::GetForward() const
{
    return m_forward;
}

FVector3 Camera::GetRight() const
{
    return m_right;
}





void Camera::UpdateViewMatrix()
{
    if (!m_followTarget)
    {
        // Convert pitch and yaw angles from degrees to radians
        float pitchRadians = MathUtil::DegreesToRadians(m_pitch);
        float yawRadians = MathUtil::DegreesToRadians(m_yaw);

        // Create quaternions representing the pitch and yaw rotations
        Quaternion pitchQuat = Quaternion::FromAxisAngle({1.0f, 0.0f, 0.0f}, pitchRadians);
        Quaternion yawQuat = Quaternion::FromAxisAngle({0.0f, 1.0f, 0.0f}, yawRadians);

        // Combine the pitch and yaw rotations
        Quaternion orientation = yawQuat * pitchQuat;

        // Rotate the default forward vector to get the new forward direction
        m_forward = orientation.RotateVector3({0.0f, 0.0f, -1.0f});
        m_right = orientation.RotateVector3({1.0f, 0.0f, 0.0f});

        // Calculate the new target position
        FVector3 target = m_position + m_forward;

        // Update the view matrix
        m_viewMatrix = Matrix4::CreateViewMatrix(m_position, target, m_worldUp);
    }
    else
    {
        m_forward = (m_target - m_position).Normalize();
        m_viewMatrix = Matrix4::CreateViewMatrix(m_position, m_target, m_worldUp);
    }
}


void Camera::UpdateProjectionMatrix()
{
    m_projectionMatrix = Matrix4::CreatePerspectiveMatrix(
        m_FOV, static_cast<float>(APP_VIRTUAL_WIDTH) / static_cast<float>(APP_INIT_WINDOW_HEIGHT), m_zNear, m_zFar);
}

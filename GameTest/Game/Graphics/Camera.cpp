#include "stdafx.h"
#include "Camera.h"
#include "Game/Math/MathUtil.h"
#include "App/SimpleSprite.h"
#include "App/AppSettings.h"

// Updated Constructor
Camera::Camera(const FVector3 &position, const FVector3 &worldUp, const Quaternion &orientation, float FOV)
    : m_position(position), m_worldUp(worldUp.Normalize()),
      m_orientation(orientation.Normalize()), // Ensure the quaternion is normalized
      m_FOV(FOV), m_zNear(0.1f), m_zFar(1000.0f), m_pitch(0.0f), m_yaw(0.0f), m_minPitch(-80.0f), m_maxPitch(80.0f)
{
    UpdateOrientation();
    UpdateAxes();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}


void Camera::LookAt(const FVector3 &target)
{
    FVector3 direction = (target - m_position).Normalize();

    // Calculate desired yaw and pitch from the direction
    float desiredYaw = (std::atan2(direction.x, direction.z));
    float desiredPitch = (std::asin(direction.y));

    // Clamp the desired pitch
    if (desiredPitch > m_maxPitch)
        desiredPitch = m_maxPitch;
    if (desiredPitch < m_minPitch)
        desiredPitch = m_minPitch;

    m_pitch = desiredPitch;
    m_yaw = desiredYaw;

    // Reconstruct orientation quaternion from clamped pitch and yaw
    UpdateOrientation();
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
    m_pitch += deltaPitch;

    // Clamp pitch to the range [m_minPitch, m_maxPitch]
    if (m_pitch > m_maxPitch)
        m_pitch = m_maxPitch;
    if (m_pitch < m_minPitch)
        m_pitch = m_minPitch;

    // Update orientation based on new pitch and yaw
    UpdateOrientation();
}

void Camera::AddYaw(float deltaYaw)
{
    m_yaw += deltaYaw;

    // Optionally wrap yaw angle to keep it within [0, 360] degrees
    if (m_yaw > 360.0f)
        m_yaw -= 360.0f;
    else if (m_yaw < 0.0f)
        m_yaw += 360.0f;

    // Update orientation
    UpdateOrientation();
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

void Camera::UpdateOrientation()
{
    // Angles are in degrees
    float pitchRad = MathUtil::DegreesToRadians(m_pitch);
    float yawRad = MathUtil::DegreesToRadians(m_yaw);

    // Negate the pitch angle to correct the rotation direction
    Quaternion pitchQuat = Quaternion::FromAxisAngle(FVector3(1.0f, 0.0f, 0.0f), -pitchRad);
    Quaternion yawQuat = Quaternion::FromAxisAngle(FVector3(0.0f, 1.0f, 0.0f), yawRad);

    // Combine yaw and pitch rotations (order matters)
    m_orientation = yawQuat * pitchQuat;

    // Normalize the orientation quaternion
    m_orientation = m_orientation.Normalize();

    // Update the camera's axes and view matrix
    UpdateAxes();
    UpdateViewMatrix();
}



// Update the View Matrix using the current orientation and position
void Camera::UpdateViewMatrix()
{
    FVector3 target = m_position + m_forward;
    if (followTarget)
    {
        //target = m_target;
    }
    m_viewMatrix = Matrix4::CreateViewMatrix(m_position, target, m_worldUp);
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
    // Apply the orientation to the default axes
    m_forward = (m_orientation * FVector3(0.0f, 0.0f, -1.0f)).Normalize();
    m_up = (m_orientation * FVector3(0.0f, 1.0f, 0.0f)).Normalize();
    m_right = m_forward.Cross(m_worldUp).Normalize();
}

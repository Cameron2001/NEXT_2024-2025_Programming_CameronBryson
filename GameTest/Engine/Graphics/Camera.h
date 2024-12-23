#pragma once
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector3.h"
#include "App/SimpleSprite.h"
#include <memory>

class Camera
{
public:
    Camera(const FVector3 &position, const FVector3 &worldUp, float yaw, float pitch, float FOV);
    ~Camera() = default;


    void SetPosition(const FVector3 &position);
    void SetYaw(float yaw);
    void SetPitch(float pitch);
    void SetFOV(float fov);

    const FVector3 &GetPosition() const;
    float GetYaw() const;
    float GetPitch() const;
    const Matrix4 &GetViewMatrix() const;
    const Matrix4 &GetProjectionMatrix() const;

private:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateAxes();

    FVector3 m_position;
    FVector3 m_forward;
    FVector3 m_up;
    FVector3 m_right;
    FVector3 m_worldUp;
    Matrix4 m_viewMatrix;
    Matrix4 m_projectionMatrix;
    float m_yaw;
    float m_pitch;
    float m_FOV;
    float m_zNear;
    float m_zFar;
};
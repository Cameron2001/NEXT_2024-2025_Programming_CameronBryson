#pragma once
#include "Game/Math/Matrix4.h"
#include "Game/Math/Vector3.h"
#include "Game/Math/Quaternion.h"
#include "App/SimpleSprite.h"
#include <memory>
class Camera
{
  public:
    Camera(const Camera &other) = default;
    Camera(Camera &&other) noexcept = default;
    Camera &operator=(const Camera &other) = default;
    Camera &operator=(Camera &&other) noexcept = default;

    Camera(const FVector3 &position);
    ~Camera() = default;

    void LookAt(const FVector3 &target);
    void SetPosition(const FVector3 &position);
    void SetFOV(float fov);
    void AddPitch(float deltaPitch);
    void AddYaw(float deltaYaw);
    const FVector3 &GetPosition() const;
    float GetFOV() const;
    const Matrix4 &GetViewMatrix() const;
    const Matrix4 &GetProjectionMatrix() const;
    FVector3 GetForward() const;
    FVector3 GetRight() const;
   

  private:
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();


    Matrix4 m_viewMatrix;
    Matrix4 m_projectionMatrix;
    FVector3 m_position;
    FVector3 m_forward = {0, 0, -1};
    FVector3 m_right = {1, 0, 0};
    FVector3 m_worldUp = {0, 1, 0};
    FVector3 m_target;
    float m_FOV = 90;
    float m_zNear;
    float m_zFar;
    float m_pitch = 0;    
    float m_yaw = 0;
    float m_minPitch; 
    float m_maxPitch;
    bool m_followTarget = false;
    
};

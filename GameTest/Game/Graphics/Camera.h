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

    Camera(const FVector3 &position, const FVector3 &worldUp, const Quaternion &orientation, float FOV);
    ~Camera() = default;

    void LookAt(const FVector3 &target);
    void SetPosition(const FVector3 &position);
    void SetFOV(float fov);
    void AddPitch(float deltaPitch);
    void AddYaw(float deltaYaw);
    const FVector3 &GetPosition() const;
    Quaternion GetOrientation() const;
    float GetFOV() const;
    const Matrix4 &GetViewMatrix() const;
    const Matrix4 &GetProjectionMatrix() const;
    FVector3 m_target;
    bool followTarget = false;
    FVector3 m_forward;
    FVector3 m_right;
    void UpdateViewMatrix();

  private:
    void UpdateOrientation();
    void UpdateProjectionMatrix();
    void UpdateAxes();


    Matrix4 m_viewMatrix;
    Matrix4 m_projectionMatrix;
    FVector3 m_position;
    FVector3 m_up;
    FVector3 m_worldUp;
    Quaternion m_orientation;
    float m_FOV;
    float m_zNear;
    float m_zFar;
    float m_pitch;    
    float m_yaw;
    float m_minPitch; 
    float m_maxPitch;
    
};

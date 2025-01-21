#pragma once

#include "Game/Math/Matrix4.h"
#include "Game/Math/Vector3.h"
#include "Game/Math/Quaternion.h"

class Camera
{
  public:
    Camera(const FVector3 &position);
    ~Camera() = default;


    void SetPosition(const FVector3 &position);
    void SetFOV(const float fov);
    void SetOrientation(const Quaternion &orientation);
    void AddPitch(const float degrees);
    void AddYaw(const float degrees);

    const FVector3 &GetPosition() const
    {
        return m_position;
    }
    float GetFOV() const
    {
        return m_FOV;
    }
    const Matrix4 &GetViewMatrix() const
    {
        return m_viewMatrix;
    }
    const Matrix4 &GetProjectionMatrix() const
    {
        return m_projectionMatrix;
    }
    FVector3 GetForward() const
    {
        return m_forward;
    }
    FVector3 GetUp() const
    {
        return m_up;
    }
    FVector3 GetRight() const
    {
        return m_right;
    }

  private:
    void UpdateViewMatrix();

    void UpdateProjectionMatrix();

    void ClampPitch();

    FVector3 m_position;      
    Quaternion m_orientation; 
    float m_FOV;              
    float m_zNear;            
    float m_zFar;             

    FVector3 m_forward; 
    FVector3 m_up;      
    FVector3 m_right;   

    FVector3 m_worldUp; 

    float m_minPitch; 
    float m_maxPitch; 

    Matrix4 m_viewMatrix;       
    Matrix4 m_projectionMatrix; 
};

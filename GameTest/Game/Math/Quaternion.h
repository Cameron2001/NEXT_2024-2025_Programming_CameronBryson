#pragma once
#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"

class Quaternion
{
  public:
    float w;
    float x;
    float y;
    float z;
    Quaternion();
    Quaternion(float x, float y, float z);
    Quaternion(float w, float x, float y, float z);
    Quaternion(const Quaternion &copy);

    ~Quaternion() = default;

    static Quaternion FromAxisAngle(const FVector3 &axis, float angleRadians);
    static Quaternion FromEulerAnglesXYZ(const FVector3 &eulerAngles);
    static Quaternion FromMatrix3(const Matrix3 &matrix);
    static Quaternion LookAt(const FVector3 &direction, const FVector3 &up);
    static Quaternion Slerp(const Quaternion &start, const Quaternion &end, float t);


    float GetPitch() const;
    float GetYaw() const;
    float GetRoll() const;
    FVector3 GetEulerAnglesXYZ() const;

    Matrix3 GetRotationMatrix3() const;
    Matrix4 GetRotationMatrix4() const;

    float Dot(const Quaternion &other) const;
    float Length() const;
    float LengthSquared() const;
    Quaternion Normalize() const;
    Quaternion Conjugate() const;
    Quaternion Inverse() const;

    FVector3 RotateVector3(const FVector3 &vector) const;

    Quaternion operator*(const Quaternion &other) const;
    Quaternion operator+(const Quaternion &other) const;
    Quaternion operator*(const float scalar) const;
    Quaternion operator/(const float scalar) const;

    bool operator==(const Quaternion &other) const;
    bool operator!=(const Quaternion &other) const;
};

#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include <cmath>

class Quaternion
{
  public:
    float w;
    float x;
    float y;
    float z;
    // Constructors
    Quaternion();
    explicit Quaternion(const FVector3 &eulerAnglesXYZ); // Euler angles in radians
    Quaternion(float w, float x, float y, float z);
    Quaternion(const Quaternion &copy);

    ~Quaternion() = default;

    static Quaternion Slerp(const Quaternion &start, const Quaternion &end, float t);

    FVector3 GetEulerAnglesXYZ() const;
    Matrix3 GetRotationMatrix3() const;
    Matrix4 GetRotationMatrix4() const;

    float Dot(const Quaternion &other) const;
    float Length() const;
    float LengthSquared() const;
    Quaternion Normalize() const;
    Quaternion Conjugate() const;
    Quaternion Inverse() const;
    void ToAxisAngle(FVector3 &axis, float &angle) const;

    Quaternion operator+(const FVector3 &obj) const;
    Quaternion &operator+=(const FVector3 &obj);

    Quaternion operator+(const Quaternion &obj) const;
    Quaternion &operator+=(const Quaternion &obj);

    Quaternion operator-(const Quaternion &obj) const;
    Quaternion &operator-=(const Quaternion &obj);

    Quaternion operator*(float scalar) const;
    Quaternion &operator*=(float scalar);

    Quaternion operator/(float scalar) const;
    Quaternion &operator/=(float scalar);

    Quaternion operator*(const Quaternion &obj) const;
    Quaternion &operator*=(const Quaternion &obj);

    FVector3 operator*(const FVector3 &v) const;

    bool operator==(const Quaternion &other) const;
    bool operator!=(const Quaternion &other) const;
};

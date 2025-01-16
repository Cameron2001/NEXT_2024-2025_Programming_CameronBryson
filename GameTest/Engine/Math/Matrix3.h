#pragma once
#include <Engine/Math/Vector3.h>
class Matrix3
{
  public:
    float m[9];

    Matrix3();
    ~Matrix3() = default;

    void Set(int row, int column, float value);
    void SetZero();
    void SetIdentity();

    FVector3 GetRight() const;
    FVector3 GetUp() const;
    FVector3 GetForward() const;

    Matrix3 Inverse() const;
    Matrix3 Transpose() const;

    Matrix3 operator*(const Matrix3 &obj) const;
    FVector3 operator*(const FVector3 &obj) const;

    bool operator==(const Matrix3 &other) const;
    bool operator!=(const Matrix3 &other) const;
};

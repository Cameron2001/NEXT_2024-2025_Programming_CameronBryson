#pragma once
#include <Game/Math/Vector3.h>
class Matrix3
{
  public:
    float m[9];

    Matrix3();
    ~Matrix3() = default;

    void Set(const int row, const int column, const float value);
    Matrix3 SetZero();
    void SetIdentity();

    float Get(const int row, const int column) const;

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

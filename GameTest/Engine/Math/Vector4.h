#pragma once

#include <algorithm>
#include <cmath>

class FVector4
{
  public:
    float X;
    float Y;
    float Z;
    float W;

    FVector4();
    FVector4(float X, float Y, float Z, float W);
    FVector4(const FVector4 &copy);

    FVector4 operator+(const FVector4 &obj) const;
    FVector4 operator-(const FVector4 &obj) const;
    FVector4 operator*(const FVector4 &obj) const;
    FVector4 operator/(const FVector4 &obj) const;
    FVector4 operator*(float obj) const;
    FVector4 operator/(float obj) const;

    FVector4 &operator+=(const FVector4 &obj);
    FVector4 &operator-=(const FVector4 &obj);
    FVector4 &operator*=(const FVector4 &obj);
    FVector4 &operator/=(const FVector4 &obj);
    FVector4 &operator*=(float obj);
    FVector4 &operator/=(float obj);

    bool operator==(const FVector4 &other) const;
    bool operator!=(const FVector4 &other) const;

    float Length() const;
    float LengthSquared() const;
    FVector4 Normalize() const;
    float Dot(const FVector4 &obj) const;
    FVector4 Clamp(float min, float max) const;
    FVector4 Project(const FVector4 &normal) const;
};

class IVector4
{
  public:
    int X;
    int Y;
    int Z;
    int W;

    IVector4();
    IVector4(int X, int Y, int Z, int W);
    IVector4(const IVector4 &copy);

    IVector4 operator+(const IVector4 &obj) const;
    IVector4 operator-(const IVector4 &obj) const;
    IVector4 operator*(const IVector4 &obj) const;
    IVector4 operator/(const IVector4 &obj) const;
    IVector4 operator*(int obj) const;
    IVector4 operator/(int obj) const;

    IVector4 &operator+=(const IVector4 &obj);
    IVector4 &operator-=(const IVector4 &obj);
    IVector4 &operator*=(const IVector4 &obj);
    IVector4 &operator/=(const IVector4 &obj);
    IVector4 &operator*=(int obj);
    IVector4 &operator/=(int obj);

    bool operator==(const IVector4 &other) const;
    bool operator!=(const IVector4 &other) const;

    float Length() const;
    float LengthSquared() const;
    IVector4 Normalize() const;
    int Dot(const IVector4 &obj) const;
    IVector4 Clamp(int min, int max) const;
    IVector4 Project(const IVector4 &normal) const;
};

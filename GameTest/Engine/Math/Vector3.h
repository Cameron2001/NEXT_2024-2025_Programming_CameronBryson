#pragma once

#include <algorithm>
#include <cmath>

class FVector3
{
  public:
    float X;
    float Y;
    float Z;

    FVector3();
    FVector3(float X, float Y, float Z);
    FVector3(const FVector3 &copy);

    // Operator Overloads
    FVector3 operator+(const FVector3 &obj) const;
    FVector3 operator-(const FVector3 &obj) const;
    FVector3 operator*(const FVector3 &obj) const;
    FVector3 operator/(const FVector3 &obj) const;
    FVector3 operator*(float obj) const;
    FVector3 operator/(float obj) const;

    FVector3 &operator+=(const FVector3 &obj);
    FVector3 &operator-=(const FVector3 &obj);
    FVector3 &operator*=(const FVector3 &obj);
    FVector3 &operator/=(const FVector3 &obj);
    FVector3 &operator*=(float obj);
    FVector3 &operator/=(float obj);

    bool operator==(const FVector3 &other) const;
    bool operator!=(const FVector3 &other) const;

    float Length() const;
    float LengthSquared() const;
    FVector3 Normalize() const;
    float Dot(const FVector3 &obj) const;
    FVector3 Cross(const FVector3 &obj) const;
};

class IVector3
{
  public:
    int X;
    int Y;
    int Z;

    IVector3();
    IVector3(int X, int Y, int Z);
    IVector3(const IVector3 &copy);

    // Operator Overloads
    IVector3 operator+(const IVector3 &obj) const;
    IVector3 operator-(const IVector3 &obj) const;
    IVector3 operator*(const IVector3 &obj) const;
    IVector3 operator/(const IVector3 &obj) const;
    IVector3 operator*(int obj) const;
    IVector3 operator/(int obj) const;

    IVector3 &operator+=(const IVector3 &obj);
    IVector3 &operator-=(const IVector3 &obj);
    IVector3 &operator*=(const IVector3 &obj);
    IVector3 &operator/=(const IVector3 &obj);
    IVector3 &operator*=(int obj);
    IVector3 &operator/=(int obj);

    float Length() const;
    float LengthSquared() const;
    IVector3 Normalize() const;
    int Dot(const IVector3 &obj) const;
    IVector3 Cross(const IVector3 &obj) const;
    IVector3 Clamp(int min, int max) const;
    IVector3 Project(const IVector3 &normal) const;
};

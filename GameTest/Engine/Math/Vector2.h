#pragma once

#include <algorithm>
#include <cmath>

class FVector2
{
  public:
    float x;
    float y;

    FVector2();
    FVector2(float X, float Y);
    FVector2(const FVector2 &copy);

    // Operator Overloads
    FVector2 operator+(const FVector2 &obj) const;
    FVector2 operator-(const FVector2 &obj) const;
    FVector2 operator*(const FVector2 &obj) const;
    FVector2 operator/(const FVector2 &obj) const;
    FVector2 operator*(float obj) const;
    FVector2 operator/(float obj) const;

    FVector2 &operator+=(const FVector2 &obj);
    FVector2 &operator-=(const FVector2 &obj);
    FVector2 &operator*=(const FVector2 &obj);
    FVector2 &operator/=(const FVector2 &obj);
    FVector2 &operator*=(float obj);
    FVector2 &operator/=(float obj);

    bool operator==(const FVector2 &other) const;
    bool operator!=(const FVector2 &other) const;

    // Member Functions
    float Length() const;
    float LengthSquared() const;
    FVector2 Normalize() const;
    float Dot(const FVector2 &other) const;
    float Cross(const FVector2 &other) const;
    FVector2 Clamp(float min, float max) const;
    FVector2 Project(const FVector2 &normal) const;
};

class IVector2
{
  public:
    int x;
    int y;

    IVector2();
    IVector2(int X, int Y);
    IVector2(const IVector2 &copy);

    // Operator Overloads
    IVector2 operator+(const IVector2 &obj) const;
    IVector2 operator-(const IVector2 &obj) const;
    IVector2 operator*(const IVector2 &obj) const;
    IVector2 operator/(const IVector2 &obj) const;
    IVector2 operator*(int obj) const;
    IVector2 operator/(int obj) const;

    IVector2 &operator+=(const IVector2 &obj);
    IVector2 &operator-=(const IVector2 &obj);
    IVector2 &operator*=(const IVector2 &obj);
    IVector2 &operator/=(const IVector2 &obj);
    IVector2 &operator*=(int obj);
    IVector2 &operator/=(int obj);

    bool operator==(const IVector2 &other) const;
    bool operator!=(const IVector2 &other) const;

    // Member Functions
    float Length() const;
    float LengthSquared() const;
    float Dot(const IVector2 &obj) const;
    float Cross(const IVector2 &obj) const;
    IVector2 Clamp(int min, int max) const;
};

#include "stdafx.h"
#include "Vector2.h"
#include <utility>
#include <cmath>

FVector2::FVector2()
{
    this->X = 0.0f;
    this->Y = 0.0f;
}

FVector2::FVector2(float X, float Y)
{
    this->X = X;
    this->Y = Y;
}

FVector2::FVector2(const FVector2 &copy)
{
    this->X = copy.X;
    this->Y = copy.Y;
}

float FVector2::Length() const
{
    return std::sqrt(X * X + Y * Y);
}

float FVector2::LengthSquared() const
{
    return X * X + Y * Y;
}

FVector2 FVector2::Normalize() const
{
    float len = Length();
    if (len > 0.0f)
    {
        return FVector2(X / len, Y / len);
    }
    return FVector2(0.0f, 0.0f); // Return zero vector if length is zero
}

float FVector2::Dot(const FVector2 &other) const
{
    return X * other.X + Y * other.Y;
}

float FVector2::Cross(const FVector2 &other) const
{
    return X * other.Y - Y * other.X;
}

FVector2 FVector2::Clamp(float min, float max) const
{
    float clampedX = std::max(min, std::min(X, max));
    float clampedY = std::max(min, std::min(Y, max));
    return FVector2(clampedX, clampedY);
}

FVector2 FVector2::Project(const FVector2 &normal) const
{
    float dotProduct = this->Dot(normal);
    float normalLengthSquared = normal.LengthSquared();
    if (normalLengthSquared > 0.0f)
    {
        float scalar = dotProduct / normalLengthSquared;
        return normal * scalar;
    }
    return FVector2(0.0f, 0.0f); // Return zero vector if normal length is zero
}
FVector2 FVector2::operator+(const FVector2 &obj) const
{
    return {this->X + obj.X, this->Y + obj.Y};
}

FVector2 FVector2::operator-(const FVector2 &obj) const
{
    return {this->X - obj.X, this->Y - obj.Y};
}

FVector2 FVector2::operator*(const FVector2 &obj) const
{
    return {this->X * obj.X, this->Y * obj.Y};
}

FVector2 FVector2::operator/(const FVector2 &obj) const
{
    return {this->X / obj.X, this->Y / obj.Y};
}

FVector2 FVector2::operator*(const float obj) const
{
    return {this->X * obj, this->Y * obj};
}

FVector2 FVector2::operator/(const float obj) const
{
    return {this->X / obj, this->Y / obj};
}

FVector2 &FVector2::operator+=(const FVector2 &obj)
{
    this->X += obj.X;
    this->Y += obj.Y;
    return *this;
}

FVector2 &FVector2::operator-=(const FVector2 &obj)
{
    this->X -= obj.X;
    this->Y -= obj.Y;
    return *this;
}

FVector2 &FVector2::operator*=(const FVector2 &obj)
{
    this->X *= obj.X;
    this->Y *= obj.Y;
    return *this;
}

FVector2 &FVector2::operator/=(const FVector2 &obj)
{
    this->X /= obj.X;
    this->Y /= obj.Y;
    return *this;
}

FVector2 &FVector2::operator*=(const float obj)
{
    this->X *= obj;
    this->Y *= obj;
    return *this;
}

FVector2 &FVector2::operator/=(const float obj)
{
    this->X /= obj;
    this->Y /= obj;
    return *this;
}

IVector2::IVector2()
{
    this->X = 0;
    this->Y = 0;
}

IVector2::IVector2(int X, int Y)
{
    this->X = X;
    this->Y = Y;
}

IVector2::IVector2(const IVector2 &copy)
{
    this->X = copy.X;
    this->Y = copy.Y;
}

IVector2 IVector2::Length()
{
    return IVector2();
}

IVector2 IVector2::LengthSquared()
{
    return IVector2();
}

IVector2 IVector2::Normalize()
{
    return IVector2();
}

IVector2 IVector2::Cross(IVector2 obj)
{
    return IVector2();
}

IVector2 IVector2::Clamp(int min, int max)
{
    return IVector2();
}

IVector2 IVector2::Project(IVector2 normal)
{
    return IVector2();
}

IVector2 IVector2::Dot(IVector2 obj)
{
    return IVector2();
}

IVector2 IVector2::operator+(const IVector2 &obj) const
{
    return {this->X + obj.X, this->Y + obj.Y};
}

IVector2 IVector2::operator-(const IVector2 &obj) const
{
    return {this->X - obj.X, this->Y - obj.Y};
}

IVector2 IVector2::operator*(const IVector2 &obj) const
{
    return {this->X * obj.X, this->Y * obj.Y};
}

IVector2 IVector2::operator/(const IVector2 &obj) const
{
    return {this->X / obj.X, this->Y / obj.Y};
}

IVector2 IVector2::operator*(const int obj) const
{
    return {this->X * obj, this->Y * obj};
}

IVector2 IVector2::operator/(const int obj) const
{
    return {this->X / obj, this->Y / obj};
}

IVector2 &IVector2::operator+=(const IVector2 &obj)
{
    this->X += obj.X;
    this->Y += obj.Y;
    return *this;
}

IVector2 &IVector2::operator-=(const IVector2 &obj)
{
    this->X -= obj.X;
    this->Y -= obj.Y;
    return *this;
}

IVector2 &IVector2::operator*=(const IVector2 &obj)
{
    this->X *= obj.X;
    this->Y *= obj.Y;
    return *this;
}

IVector2 &IVector2::operator/=(const IVector2 &obj)
{
    this->X /= obj.X;
    this->Y /= obj.Y;
    return *this;
}

IVector2 &IVector2::operator*=(const int obj)
{
    this->X *= obj;
    this->Y *= obj;
    return *this;
}

IVector2 &IVector2::operator/=(const int obj)
{
    this->X /= obj;
    this->Y /= obj;
    return *this;
}
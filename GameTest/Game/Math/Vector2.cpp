#include "stdafx.h"
#include "Vector2.h"

FVector2::FVector2() : x(0.0f), y(0.0f)
{
}

FVector2::FVector2(const float X, const float Y) : x(X), y(Y)
{
}

FVector2::FVector2(const FVector2 &copy) : x(copy.x), y(copy.y)
{
}

FVector2 FVector2::operator+(const FVector2 &obj) const
{
    return FVector2(x + obj.x, y + obj.y);
}

FVector2 FVector2::operator-(const FVector2 &obj) const
{
    return FVector2(x - obj.x, y - obj.y);
}

FVector2 FVector2::operator*(const FVector2 &obj) const
{
    return FVector2(x * obj.x, y * obj.y);
}

FVector2 FVector2::operator/(const FVector2 &obj) const
{
    return FVector2(x / obj.x, y / obj.y);
}

FVector2 FVector2::operator*(const float obj) const
{
    return FVector2(x * obj, y * obj);
}

FVector2 FVector2::operator/(const float obj) const
{
    return FVector2(x / obj, y / obj);
}

FVector2 &FVector2::operator+=(const FVector2 &obj)
{
    x += obj.x;
    y += obj.y;
    return *this;
}

FVector2 &FVector2::operator-=(const FVector2 &obj)
{
    x -= obj.x;
    y -= obj.y;
    return *this;
}

FVector2 &FVector2::operator*=(const FVector2 &obj)
{
    x *= obj.x;
    y *= obj.y;
    return *this;
}

FVector2 &FVector2::operator/=(const FVector2 &obj)
{
    x /= obj.x;
    y /= obj.y;
    return *this;
}

FVector2 &FVector2::operator*=(const float obj)
{
    x *= obj;
    y *= obj;
    return *this;
}

FVector2 &FVector2::operator/=(const float obj)
{
    x /= obj;
    y /= obj;
    return *this;
}

bool FVector2::operator==(const FVector2 &other) const
{
    constexpr float EPSILON = 1e-5f;
    return (std::fabs(x - other.x) < EPSILON) && (std::fabs(y - other.y) < EPSILON);
}

bool FVector2::operator!=(const FVector2 &other) const
{
    return !(*this == other);
}

float FVector2::Length() const
{
    return std::sqrt(x * x + y * y);
}

float FVector2::LengthSquared() const
{
    return x * x + y * y;
}

FVector2 FVector2::Normalize() const
{
    float len = Length();
    if (len > 0.0f)
    {
        return FVector2(x / len, y / len);
    }
    return FVector2(0.0f, 0.0f);
}

float FVector2::Dot(const FVector2 &other) const
{
    return x * other.x + y * other.y;
}

float FVector2::Cross(const FVector2 &other) const
{
    return x * other.y - y * other.x;
}

FVector2 FVector2::Clamp(const float min, const float max) const
{
    const float clampedX = std::max(min, std::min(x, max));
    const float clampedY = std::max(min, std::min(y, max));
    return FVector2(clampedX, clampedY);
}

FVector2 FVector2::Project(const FVector2 &normal) const
{
    const float dotProduct = Dot(normal);
    const float normalLengthSquared = normal.LengthSquared();
    if (normalLengthSquared > 0.0f)
    {
        const float scalar = dotProduct / normalLengthSquared;
        return normal * scalar;
    }
    return FVector2(0.0f, 0.0f);
}

IVector2::IVector2() : x(0), y(0)
{
}

IVector2::IVector2(const int X, const int Y) : x(X), y(Y)
{
}

IVector2::IVector2(const IVector2 &copy) : x(copy.x), y(copy.y)
{
}

IVector2 IVector2::operator+(const IVector2 &obj) const
{
    return IVector2(x + obj.x, y + obj.y);
}

IVector2 IVector2::operator-(const IVector2 &obj) const
{
    return IVector2(x - obj.x, y - obj.y);
}

IVector2 IVector2::operator*(const IVector2 &obj) const
{
    return IVector2(x * obj.x, y * obj.y);
}

IVector2 IVector2::operator/(const IVector2 &obj) const
{
    return IVector2(x / obj.x, y / obj.y);
}

IVector2 IVector2::operator*(const int obj) const
{
    return IVector2(x * obj, y * obj);
}

IVector2 IVector2::operator/(const int obj) const
{
    return IVector2(x / obj, y / obj);
}

IVector2 &IVector2::operator+=(const IVector2 &obj)
{
    x += obj.x;
    y += obj.y;
    return *this;
}

IVector2 &IVector2::operator-=(const IVector2 &obj)
{
    x -= obj.x;
    y -= obj.y;
    return *this;
}

IVector2 &IVector2::operator*=(const IVector2 &obj)
{
    x *= obj.x;
    y *= obj.y;
    return *this;
}

IVector2 &IVector2::operator/=(const IVector2 &obj)
{
    x /= obj.x;
    y /= obj.y;
    return *this;
}

IVector2 &IVector2::operator*=(const int obj)
{
    x *= obj;
    y *= obj;
    return *this;
}

IVector2 &IVector2::operator/=(const int obj)
{
    x /= obj;
    y /= obj;
    return *this;
}

bool IVector2::operator==(const IVector2 &other) const
{
    return x == other.x && y == other.y;
}

bool IVector2::operator!=(const IVector2 &other) const
{
    return !(*this == other);
}

float IVector2::Length() const
{
    return std::sqrt(static_cast<float>(x * x + y * y));
}

float IVector2::LengthSquared() const
{
    return static_cast<float>(x * x + y * y);
}

float IVector2::Dot(const IVector2 &obj) const
{
    return static_cast<float>(x * obj.x + y * obj.y);
}

float IVector2::Cross(const IVector2 &obj) const
{
    return static_cast<float>(x * obj.y - y * obj.x);
}

IVector2 IVector2::Clamp(int min, int max) const
{
    const int clampedX = std::max(min, std::min(x, max));
    const int clampedY = std::max(min, std::min(y, max));
    return IVector2(clampedX, clampedY);
}

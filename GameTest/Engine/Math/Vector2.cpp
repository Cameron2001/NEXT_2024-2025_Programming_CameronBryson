#include "stdafx.h"
#include "Vector2.h"

FVector2::FVector2() : X(0.0f), Y(0.0f)
{
}

FVector2::FVector2(const float X, const float Y) : X(X), Y(Y)
{
}

FVector2::FVector2(const FVector2 &copy) : X(copy.X), Y(copy.Y)
{
}

FVector2 FVector2::operator+(const FVector2 &obj) const
{
    return FVector2(X + obj.X, Y + obj.Y);
}

FVector2 FVector2::operator-(const FVector2 &obj) const
{
    return FVector2(X - obj.X, Y - obj.Y);
}

FVector2 FVector2::operator*(const FVector2 &obj) const
{
    return FVector2(X * obj.X, Y * obj.Y);
}

FVector2 FVector2::operator/(const FVector2 &obj) const
{
    return FVector2(X / obj.X, Y / obj.Y);
}

FVector2 FVector2::operator*(const float obj) const
{
    return FVector2(X * obj, Y * obj);
}

FVector2 FVector2::operator/(const float obj) const
{
    return FVector2(X / obj, Y / obj);
}

FVector2 &FVector2::operator+=(const FVector2 &obj)
{
    X += obj.X;
    Y += obj.Y;
    return *this;
}

FVector2 &FVector2::operator-=(const FVector2 &obj)
{
    X -= obj.X;
    Y -= obj.Y;
    return *this;
}

FVector2 &FVector2::operator*=(const FVector2 &obj)
{
    X *= obj.X;
    Y *= obj.Y;
    return *this;
}

FVector2 &FVector2::operator/=(const FVector2 &obj)
{
    X /= obj.X;
    Y /= obj.Y;
    return *this;
}

FVector2 &FVector2::operator*=(const float obj)
{
    X *= obj;
    Y *= obj;
    return *this;
}

FVector2 &FVector2::operator/=(const float obj)
{
    X /= obj;
    Y /= obj;
    return *this;
}

bool FVector2::operator==(const FVector2 &other) const
{
    constexpr float EPSILON = 1e-5f;
    return (std::fabs(X - other.X) < EPSILON) && (std::fabs(Y - other.Y) < EPSILON);
}

bool FVector2::operator!=(const FVector2 &other) const
{
    return !(*this == other);
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
    return FVector2(0.0f, 0.0f);
}

float FVector2::Dot(const FVector2 &other) const
{
    return X * other.X + Y * other.Y;
}

float FVector2::Cross(const FVector2 &other) const
{
    return X * other.Y - Y * other.X;
}

FVector2 FVector2::Clamp(const float min, const float max) const
{
    const float clampedX = std::max(min, std::min(X, max));
    const float clampedY = std::max(min, std::min(Y, max));
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


IVector2::IVector2() : X(0), Y(0)
{
}

IVector2::IVector2(const int X, const int Y) : X(X), Y(Y)
{
}

IVector2::IVector2(const IVector2 &copy) : X(copy.X), Y(copy.Y)
{
}

IVector2 IVector2::operator+(const IVector2 &obj) const
{
    return IVector2(X + obj.X, Y + obj.Y);
}

IVector2 IVector2::operator-(const IVector2 &obj) const
{
    return IVector2(X - obj.X, Y - obj.Y);
}

IVector2 IVector2::operator*(const IVector2 &obj) const
{
    return IVector2(X * obj.X, Y * obj.Y);
}

IVector2 IVector2::operator/(const IVector2 &obj) const
{
    return IVector2(X / obj.X, Y / obj.Y);
}

IVector2 IVector2::operator*(const int obj) const
{
    return IVector2(X * obj, Y * obj);
}

IVector2 IVector2::operator/(const int obj) const
{
    return IVector2(X / obj, Y / obj);
}

IVector2 &IVector2::operator+=(const IVector2 &obj)
{
    X += obj.X;
    Y += obj.Y;
    return *this;
}

IVector2 &IVector2::operator-=(const IVector2 &obj)
{
    X -= obj.X;
    Y -= obj.Y;
    return *this;
}

IVector2 &IVector2::operator*=(const IVector2 &obj)
{
    X *= obj.X;
    Y *= obj.Y;
    return *this;
}

IVector2 &IVector2::operator/=(const IVector2 &obj)
{
    X /= obj.X;
    Y /= obj.Y;
    return *this;
}

IVector2 &IVector2::operator*=(const int obj)
{
    X *= obj;
    Y *= obj;
    return *this;
}

IVector2 &IVector2::operator/=(const int obj)
{
    X /= obj;
    Y /= obj;
    return *this;
}

float IVector2::Length() const
{
    return std::sqrt(static_cast<float>(X * X + Y * Y));
}

float IVector2::LengthSquared() const
{
    return static_cast<float>(X * X + Y * Y);
}

IVector2 IVector2::Normalize() const
{
    const float len = Length();
    if (len > 0.0f)
    {
        return IVector2(static_cast<int>(X / len), static_cast<int>(Y / len));
    }
    return IVector2(0, 0);
}

float IVector2::Dot(const IVector2 &obj) const
{
    return static_cast<float>(X * obj.X + Y * obj.Y);
}

float IVector2::Cross(const IVector2 &obj) const
{
    return static_cast<float>(X * obj.Y - Y * obj.X);
}

IVector2 IVector2::Clamp(int min, int max) const
{
    const int clampedX = std::max(min, std::min(X, max));
    const int clampedY = std::max(min, std::min(Y, max));
    return IVector2(clampedX, clampedY);
}

IVector2 IVector2::Project(const IVector2 &normal) const
{
    const float dotProduct = Dot(normal);
    const float normalLengthSquared = normal.LengthSquared();
    if (normalLengthSquared > 0.0f)
    {
        const float scalar = dotProduct / normalLengthSquared;
        return IVector2(static_cast<int>(normal.X * scalar), static_cast<int>(normal.Y * scalar));
    }
    return IVector2(0, 0);
}

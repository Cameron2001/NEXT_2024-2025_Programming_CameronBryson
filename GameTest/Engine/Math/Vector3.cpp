#include "stdafx.h"
#include "Vector3.h"

FVector3::FVector3() : X(0.0f), Y(0.0f), Z(0.0f)
{
}

FVector3::FVector3(const float X, const float Y, const float Z) : X(X), Y(Y), Z(Z)
{
}

FVector3::FVector3(const FVector3 &copy) : X(copy.X), Y(copy.Y), Z(copy.Z)
{
}

FVector3 FVector3::operator+(const FVector3 &obj) const
{
    return FVector3(X + obj.X, Y + obj.Y, Z + obj.Z);
}

FVector3 FVector3::operator-(const FVector3 &obj) const
{
    return FVector3(X - obj.X, Y - obj.Y, Z - obj.Z);
}

FVector3 FVector3::operator*(const FVector3 &obj) const
{
    return FVector3(X * obj.X, Y * obj.Y, Z * obj.Z);
}

FVector3 FVector3::operator/(const FVector3 &obj) const
{
    return FVector3(X / obj.X, Y / obj.Y, Z / obj.Z);
}

FVector3 FVector3::operator*(const float obj) const
{
    return FVector3(X * obj, Y * obj, Z * obj);
}

FVector3 FVector3::operator/(const float obj) const
{
    return FVector3(X / obj, Y / obj, Z / obj);
}

FVector3 &FVector3::operator+=(const FVector3 &obj)
{
    X += obj.X;
    Y += obj.Y;
    Z += obj.Z;
    return *this;
}

FVector3 &FVector3::operator-=(const FVector3 &obj)
{
    X -= obj.X;
    Y -= obj.Y;
    Z -= obj.Z;
    return *this;
}

FVector3 &FVector3::operator*=(const FVector3 &obj)
{
    X *= obj.X;
    Y *= obj.Y;
    Z *= obj.Z;
    return *this;
}

FVector3 &FVector3::operator/=(const FVector3 &obj)
{
    X /= obj.X;
    Y /= obj.Y;
    Z /= obj.Z;
    return *this;
}

FVector3 &FVector3::operator*=(const float obj)
{
    X *= obj;
    Y *= obj;
    Z *= obj;
    return *this;
}

FVector3 &FVector3::operator/=(const float obj)
{
    X /= obj;
    Y /= obj;
    Z /= obj;
    return *this;
}

bool FVector3::operator==(const FVector3 &other) const
{
    constexpr float EPSILON = 1e-5f;
    return (std::fabs(X - other.X) < EPSILON) && (std::fabs(Y - other.Y) < EPSILON) &&
           (std::fabs(Z - other.Z) < EPSILON);
}

bool FVector3::operator!=(const FVector3 &other) const
{
    return !(*this == other);
}

float FVector3::Length() const
{
    return std::sqrt(X * X + Y * Y + Z * Z);
}

float FVector3::LengthSquared() const
{
    return X * X + Y * Y + Z * Z;
}

FVector3 FVector3::Normalize() const
{
    const float len = Length();
    if (len > 0.0f)
    {
        return FVector3(X / len, Y / len, Z / len);
    }
    return FVector3(0.0f, 0.0f, 0.0f);
}

float FVector3::Dot(const FVector3 &obj) const
{
    return X * obj.X + Y * obj.Y + Z * obj.Z;
}

FVector3 FVector3::Cross(const FVector3 &obj) const
{
    return FVector3(Y * obj.Z - Z * obj.Y, Z * obj.X - X * obj.Z, X * obj.Y - Y * obj.X);
}

FVector3 FVector3::Clamp(float min, float max) const
{
    float clampedX = std::max(min, std::min(X, max));
    float clampedY = std::max(min, std::min(Y, max));
    float clampedZ = std::max(min, std::min(Z, max));
    return FVector3(clampedX, clampedY, clampedZ);
}

FVector3 FVector3::Project(const FVector3 &normal) const
{
    float dotProduct = Dot(normal);
    float normalLengthSquared = normal.LengthSquared();
    if (normalLengthSquared > 0.0f)
    {
        float scalar = dotProduct / normalLengthSquared;
        return FVector3(normal.X * scalar, normal.Y * scalar, normal.Z * scalar);
    }
    return FVector3(0.0f, 0.0f, 0.0f);
}

IVector3::IVector3() : X(0), Y(0), Z(0)
{
}

IVector3::IVector3(const int X, const int Y, const int Z) : X(X), Y(Y), Z(Z)
{
}

IVector3::IVector3(const IVector3 &copy) : X(copy.X), Y(copy.Y), Z(copy.Z)
{
}

IVector3 IVector3::operator+(const IVector3 &obj) const
{
    return IVector3(X + obj.X, Y + obj.Y, Z + obj.Z);
}

IVector3 IVector3::operator-(const IVector3 &obj) const
{
    return IVector3(X - obj.X, Y - obj.Y, Z - obj.Z);
}

IVector3 IVector3::operator*(const IVector3 &obj) const
{
    return IVector3(X * obj.X, Y * obj.Y, Z * obj.Z);
}

IVector3 IVector3::operator/(const IVector3 &obj) const
{
    return IVector3(X / obj.X, Y / obj.Y, Z / obj.Z);
}

IVector3 IVector3::operator*(const int obj) const
{
    return IVector3(X * obj, Y * obj, Z * obj);
}

IVector3 IVector3::operator/(const int obj) const
{
    return IVector3(X / obj, Y / obj, Z / obj);
}

IVector3 &IVector3::operator+=(const IVector3 &obj)
{
    X += obj.X;
    Y += obj.Y;
    Z += obj.Z;
    return *this;
}

IVector3 &IVector3::operator-=(const IVector3 &obj)
{
    X -= obj.X;
    Y -= obj.Y;
    Z -= obj.Z;
    return *this;
}

IVector3 &IVector3::operator*=(const IVector3 &obj)
{
    X *= obj.X;
    Y *= obj.Y;
    Z *= obj.Z;
    return *this;
}

IVector3 &IVector3::operator/=(const IVector3 &obj)
{
    X /= obj.X;
    Y /= obj.Y;
    Z /= obj.Z;
    return *this;
}

IVector3 &IVector3::operator*=(const int obj)
{
    X *= obj;
    Y *= obj;
    Z *= obj;
    return *this;
}

IVector3 &IVector3::operator/=(const int obj)
{
    X /= obj;
    Y /= obj;
    Z /= obj;
    return *this;
}

bool IVector3::operator==(const IVector3 &other) const
{
    return X == other.X && Y == other.Y && Z == other.Z;
}

bool IVector3::operator!=(const IVector3 &other) const
{
    return !(*this == other);
}

float IVector3::Length() const
{
    return std::sqrt(static_cast<float>(X * X + Y * Y + Z * Z));
}

float IVector3::LengthSquared() const
{
    return static_cast<float>(X * X + Y * Y + Z * Z);
}

IVector3 IVector3::Normalize() const
{
    const float len = Length();
    if (len > 0.0f)
    {
        return IVector3(static_cast<int>(X / len), static_cast<int>(Y / len), static_cast<int>(Z / len));
    }
    return IVector3(0, 0, 0);
}

int IVector3::Dot(const IVector3 &obj) const
{
    return X * obj.X + Y * obj.Y + Z * obj.Z;
}

IVector3 IVector3::Cross(const IVector3 &obj) const
{
    return IVector3(Y * obj.Z - Z * obj.Y, Z * obj.X - X * obj.Z, X * obj.Y - Y * obj.X);
}

IVector3 IVector3::Clamp(const int min, const int max) const
{
    const int clampedX = std::max(min, std::min(X, max));
    const int clampedY = std::max(min, std::min(Y, max));
    const int clampedZ = std::max(min, std::min(Z, max));
    return IVector3(clampedX, clampedY, clampedZ);
}

IVector3 IVector3::Project(const IVector3 &normal) const
{
    const int dotProduct = Dot(normal);
    const int normalLengthSquared = normal.X * normal.X + normal.Y * normal.Y + normal.Z * normal.Z;
    if (normalLengthSquared > 0)
    {
        const float scalar = static_cast<float>(dotProduct) / normalLengthSquared;
        return IVector3(static_cast<int>(normal.X * scalar), static_cast<int>(normal.Y * scalar),
                        static_cast<int>(normal.Z * scalar));
    }
    return IVector3(0, 0, 0);
}

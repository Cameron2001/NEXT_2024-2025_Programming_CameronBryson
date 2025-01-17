#include "stdafx.h"
#include "Vector3.h"

FVector3::FVector3() : x(0.0f), y(0.0f), z(0.0f)
{
}

FVector3::FVector3(const float X, const float Y, const float Z) : x(X), y(Y), z(Z)
{
}

FVector3::FVector3(const FVector3 &copy) : x(copy.x), y(copy.y), z(copy.z)
{
}

FVector3 FVector3::operator+(const FVector3 &obj) const
{
    return FVector3(x + obj.x, y + obj.y, z + obj.z);
}

FVector3 FVector3::operator-(const FVector3 &obj) const
{
    return FVector3(x - obj.x, y - obj.y, z - obj.z);
}

FVector3 FVector3::operator*(const FVector3 &obj) const
{
    return FVector3(x * obj.x, y * obj.y, z * obj.z);
}

FVector3 FVector3::operator/(const FVector3 &obj) const
{
    return FVector3(x / obj.x, y / obj.y, z / obj.z);
}

FVector3 FVector3::operator*(const float obj) const
{
    return FVector3(x * obj, y * obj, z * obj);
}

FVector3 FVector3::operator/(const float obj) const
{
    return FVector3(x / obj, y / obj, z / obj);
}

FVector3 FVector3::operator-() const
{
    return {-x, -y, -z};
}

FVector3 &FVector3::operator+=(const FVector3 &obj)
{
    x += obj.x;
    y += obj.y;
    z += obj.z;
    return *this;
}

FVector3 &FVector3::operator-=(const FVector3 &obj)
{
    x -= obj.x;
    y -= obj.y;
    z -= obj.z;
    return *this;
}

FVector3 &FVector3::operator*=(const FVector3 &obj)
{
    x *= obj.x;
    y *= obj.y;
    z *= obj.z;
    return *this;
}

FVector3 &FVector3::operator/=(const FVector3 &obj)
{
    x /= obj.x;
    y /= obj.y;
    z /= obj.z;
    return *this;
}

FVector3 &FVector3::operator*=(const float obj)
{
    x *= obj;
    y *= obj;
    z *= obj;
    return *this;
}

FVector3 &FVector3::operator/=(const float obj)
{
    x /= obj;
    y /= obj;
    z /= obj;
    return *this;
}

bool FVector3::operator==(const FVector3 &other) const
{
    constexpr float EPSILON = 1e-5f;
    return (std::fabs(x - other.x) < EPSILON) && (std::fabs(y - other.y) < EPSILON) &&
           (std::fabs(z - other.z) < EPSILON);
}

bool FVector3::operator!=(const FVector3 &other) const
{
    return !(*this == other);
}

float FVector3::Length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

float FVector3::LengthSquared() const
{
    return x * x + y * y + z * z;
}

FVector3 FVector3::Normalize() const
{
    const float len = Length();
    if (len > 0.0f)
    {
        return FVector3(x / len, y / len, z / len);
    }
    return FVector3(0.0f, 0.0f, 0.0f);
}

float FVector3::Dot(const FVector3 &obj) const
{
    return x * obj.x + y * obj.y + z * obj.z;
}

FVector3 FVector3::Cross(const FVector3 &obj) const
{
    return FVector3(y * obj.z - z * obj.y, z * obj.x - x * obj.z, x * obj.y - y * obj.x);
}

FVector3 FVector3::Clamp(float min, float max) const
{
    float clampedX = std::max(min, std::min(x, max));
    float clampedY = std::max(min, std::min(y, max));
    float clampedZ = std::max(min, std::min(z, max));
    return FVector3(clampedX, clampedY, clampedZ);
}
FVector3 FVector3::Clamp(const FVector3 &min, const FVector3 &max) const
{
    float clampedX = std::max(min.x, std::min(x, max.x));
    float clampedY = std::max(min.y, std::min(y, max.y));
    float clampedZ = std::max(min.z, std::min(z, max.z));
    return FVector3(clampedX, clampedY, clampedZ);
}
FVector3 FVector3::Project(const FVector3 &normal) const
{
    float dotProduct = Dot(normal);
    float normalLengthSquared = normal.LengthSquared();
    if (normalLengthSquared > 0.0f)
    {
        float scalar = dotProduct / normalLengthSquared;
        return FVector3(normal.x * scalar, normal.y * scalar, normal.z * scalar);
    }
    return FVector3(0.0f, 0.0f, 0.0f);
}

IVector3::IVector3() : x(0), y(0), z(0)
{
}

IVector3::IVector3(const int X, const int Y, const int Z) : x(X), y(Y), z(Z)
{
}

IVector3::IVector3(const IVector3 &copy) : x(copy.x), y(copy.y), z(copy.z)
{
}

IVector3 IVector3::operator+(const IVector3 &obj) const
{
    return IVector3(x + obj.x, y + obj.y, z + obj.z);
}

IVector3 IVector3::operator-(const IVector3 &obj) const
{
    return IVector3(x - obj.x, y - obj.y, z - obj.z);
}

IVector3 IVector3::operator*(const IVector3 &obj) const
{
    return IVector3(x * obj.x, y * obj.y, z * obj.z);
}

IVector3 IVector3::operator/(const IVector3 &obj) const
{
    return IVector3(x / obj.x, y / obj.y, z / obj.z);
}

IVector3 IVector3::operator*(const int obj) const
{
    return IVector3(x * obj, y * obj, z * obj);
}

IVector3 IVector3::operator/(const int obj) const
{
    return IVector3(x / obj, y / obj, z / obj);
}

IVector3 &IVector3::operator+=(const IVector3 &obj)
{
    x += obj.x;
    y += obj.y;
    z += obj.z;
    return *this;
}

IVector3 &IVector3::operator-=(const IVector3 &obj)
{
    x -= obj.x;
    y -= obj.y;
    z -= obj.z;
    return *this;
}

IVector3 &IVector3::operator*=(const IVector3 &obj)
{
    x *= obj.x;
    y *= obj.y;
    z *= obj.z;
    return *this;
}

IVector3 &IVector3::operator/=(const IVector3 &obj)
{
    x /= obj.x;
    y /= obj.y;
    z /= obj.z;
    return *this;
}

IVector3 &IVector3::operator*=(const int obj)
{
    x *= obj;
    y *= obj;
    z *= obj;
    return *this;
}

IVector3 &IVector3::operator/=(const int obj)
{
    x /= obj;
    y /= obj;
    z /= obj;
    return *this;
}

bool IVector3::operator==(const IVector3 &other) const
{
    return x == other.x && y == other.y && z == other.z;
}

bool IVector3::operator!=(const IVector3 &other) const
{
    return !(*this == other);
}

float IVector3::Length() const
{
    return std::sqrt(static_cast<float>(x * x + y * y + z * z));
}

float IVector3::LengthSquared() const
{
    return static_cast<float>(x * x + y * y + z * z);
}

int IVector3::Dot(const IVector3 &obj) const
{
    return x * obj.x + y * obj.y + z * obj.z;
}

IVector3 IVector3::Cross(const IVector3 &obj) const
{
    return IVector3(y * obj.z - z * obj.y, z * obj.x - x * obj.z, x * obj.y - y * obj.x);
}

IVector3 IVector3::Clamp(const int min, const int max) const
{
    const int clampedX = std::max(min, std::min(x, max));
    const int clampedY = std::max(min, std::min(y, max));
    const int clampedZ = std::max(min, std::min(z, max));
    return IVector3(clampedX, clampedY, clampedZ);
}

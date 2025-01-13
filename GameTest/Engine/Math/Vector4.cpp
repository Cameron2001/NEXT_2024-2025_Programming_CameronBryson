#include "stdafx.h"
#include "Vector4.h"

// FVector4 Implementations

FVector4::FVector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

FVector4::FVector4(const float X, const float Y, const float Z, const float W) : x(X), y(Y), z(Z), w(W)
{
}

FVector4::FVector4(const FVector4 &copy) : x(copy.x), y(copy.y), z(copy.z), w(copy.w)
{
}

FVector4 FVector4::operator+(const FVector4 &obj) const
{
    return FVector4(x + obj.x, y + obj.y, z + obj.z, w + obj.w);
}

FVector4 FVector4::operator-(const FVector4 &obj) const
{
    return FVector4(x - obj.x, y - obj.y, z - obj.z, w - obj.w);
}

FVector4 FVector4::operator*(const FVector4 &obj) const
{
    return FVector4(x * obj.x, y * obj.y, z * obj.z, w * obj.w);
}

FVector4 FVector4::operator/(const FVector4 &obj) const
{
    return FVector4(obj.x != 0.0f ? x / obj.x : 0.0f, obj.y != 0.0f ? y / obj.y : 0.0f,
                    obj.z != 0.0f ? z / obj.z : 0.0f, obj.w != 0.0f ? w / obj.w : 0.0f);
}

FVector4 FVector4::operator*(const float obj) const
{
    return FVector4(x * obj, y * obj, z * obj, w * obj);
}

FVector4 FVector4::operator/(const float obj) const
{
    if (obj != 0.0f)
    {
        return FVector4(x / obj, y / obj, z / obj, w / obj);
    }
    return FVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

FVector4 &FVector4::operator+=(const FVector4 &obj)
{
    x += obj.x;
    y += obj.y;
    z += obj.z;
    w += obj.w;
    return *this;
}

FVector4 &FVector4::operator-=(const FVector4 &obj)
{
    x -= obj.x;
    y -= obj.y;
    z -= obj.z;
    w -= obj.w;
    return *this;
}

FVector4 &FVector4::operator*=(const FVector4 &obj)
{
    x *= obj.x;
    y *= obj.y;
    z *= obj.z;
    w *= obj.w;
    return *this;
}

FVector4 &FVector4::operator/=(const FVector4 &obj)
{
    if (obj.x != 0.0f)
        x /= obj.x;
    else
        x = 0.0f;
    if (obj.y != 0.0f)
        y /= obj.y;
    else
        y = 0.0f;
    if (obj.z != 0.0f)
        z /= obj.z;
    else
        z = 0.0f;
    if (obj.w != 0.0f)
        w /= obj.w;
    else
        w = 0.0f;
    return *this;
}

FVector4 &FVector4::operator*=(const float obj)
{
    x *= obj;
    y *= obj;
    z *= obj;
    w *= obj;
    return *this;
}

FVector4 &FVector4::operator/=(const float obj)
{
    if (obj != 0.0f)
    {
        x /= obj;
        y /= obj;
        z /= obj;
        w /= obj;
    }
    else
    {
        x = y = z = w = 0.0f;
    }
    return *this;
}

bool FVector4::operator==(const FVector4 &other) const
{
    constexpr float EPSILON = 1e-5f;
    return (std::fabs(x - other.x) < EPSILON) && (std::fabs(y - other.y) < EPSILON) &&
           (std::fabs(z - other.z) < EPSILON) && (std::fabs(w - other.w) < EPSILON);
}

bool FVector4::operator!=(const FVector4 &other) const
{
    return !(*this == other);
}

float FVector4::Length() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

float FVector4::LengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

FVector4 FVector4::Normalize() const
{
    const float len = Length();
    if (len > 0.0f)
    {
        return FVector4(x / len, y / len, z / len, w / len);
    }
    return FVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

float FVector4::Dot(const FVector4 &obj) const
{
    return x * obj.x + y * obj.y + z * obj.z + w * obj.w;
}

FVector4 FVector4::Clamp(const float min, const float max) const
{
    return FVector4(std::max(min, std::min(x, max)), std::max(min, std::min(y, max)), std::max(min, std::min(z, max)),
                    std::max(min, std::min(w, max)));
}

FVector4 FVector4::Project(const FVector4 &normal) const
{
    const float normalLengthSquared = normal.Dot(normal);
    if (normalLengthSquared > 0.0f)
    {
        const float scalar = this->Dot(normal) / normalLengthSquared;
        return normal * scalar;
    }
    return FVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

// IVector4 Implementations

IVector4::IVector4() : x(0), y(0), z(0), w(0)
{
}

IVector4::IVector4(const int X, const int Y, const int Z, const int W) : x(X), y(Y), z(Z), w(W)
{
}

IVector4::IVector4(const IVector4 &copy) : x(copy.x), y(copy.y), z(copy.z), w(copy.w)
{
}

IVector4 IVector4::operator+(const IVector4 &obj) const
{
    return IVector4(x + obj.x, y + obj.y, z + obj.z, w + obj.w);
}

IVector4 IVector4::operator-(const IVector4 &obj) const
{
    return IVector4(x - obj.x, y - obj.y, z - obj.z, w - obj.w);
}

IVector4 IVector4::operator*(const IVector4 &obj) const
{
    return IVector4(x * obj.x, y * obj.y, z * obj.z, w * obj.w);
}

IVector4 IVector4::operator/(const IVector4 &obj) const
{
    return IVector4(obj.x != 0 ? x / obj.x : 0, obj.y != 0 ? y / obj.y : 0, obj.z != 0 ? z / obj.z : 0,
                    obj.w != 0 ? w / obj.w : 0);
}

IVector4 IVector4::operator*(const int obj) const
{
    return IVector4(x * obj, y * obj, z * obj, w * obj);
}

IVector4 IVector4::operator/(const int obj) const
{
    if (obj != 0)
    {
        return IVector4(x / obj, y / obj, z / obj, w / obj);
    }
    return IVector4(0, 0, 0, 0);
}

IVector4 &IVector4::operator+=(const IVector4 &obj)
{
    x += obj.x;
    y += obj.y;
    z += obj.z;
    w += obj.w;
    return *this;
}

IVector4 &IVector4::operator-=(const IVector4 &obj)
{
    x -= obj.x;
    y -= obj.y;
    z -= obj.z;
    w -= obj.w;
    return *this;
}

IVector4 &IVector4::operator*=(const IVector4 &obj)
{
    x *= obj.x;
    y *= obj.y;
    z *= obj.z;
    w *= obj.w;
    return *this;
}

IVector4 &IVector4::operator/=(const IVector4 &obj)
{
    if (obj.x != 0)
        x /= obj.x;
    else
        x = 0;
    if (obj.y != 0)
        y /= obj.y;
    else
        y = 0;
    if (obj.z != 0)
        z /= obj.z;
    else
        z = 0;
    if (obj.w != 0)
        w /= obj.w;
    else
        w = 0;
    return *this;
}

IVector4 &IVector4::operator*=(const int obj)
{
    x *= obj;
    y *= obj;
    z *= obj;
    w *= obj;
    return *this;
}

IVector4 &IVector4::operator/=(const int obj)
{
    if (obj != 0)
    {
        x /= obj;
        y /= obj;
        z /= obj;
        w /= obj;
    }
    else
    {
        x = y = z = w = 0;
    }
    return *this;
}

bool IVector4::operator==(const IVector4 &other) const
{
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool IVector4::operator!=(const IVector4 &other) const
{
    return !(*this == other);
}

float IVector4::Length() const
{
    return std::sqrt(static_cast<float>(x * x + y * y + z * z + w * w));
}

float IVector4::LengthSquared() const
{
    return static_cast<float>(x * x + y * y + z * z + w * w);
}

int IVector4::Dot(const IVector4 &obj) const
{
    return x * obj.x + y * obj.y + z * obj.z + w * obj.w;
}

IVector4 IVector4::Clamp(const int min, const int max) const
{
    return IVector4(std::max(min, std::min(x, max)), std::max(min, std::min(y, max)), std::max(min, std::min(z, max)),
                    std::max(min, std::min(w, max)));
}

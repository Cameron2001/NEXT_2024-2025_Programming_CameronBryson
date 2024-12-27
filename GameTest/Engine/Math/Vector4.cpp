#include "stdafx.h"
#include "Vector4.h"

// FVector4 Implementations

FVector4::FVector4() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
{
}

FVector4::FVector4(const float X, const float Y, const float Z, const float W) : X(X), Y(Y), Z(Z), W(W)
{
}

FVector4::FVector4(const FVector4 &copy) : X(copy.X), Y(copy.Y), Z(copy.Z), W(copy.W)
{
}

FVector4 FVector4::operator+(const FVector4 &obj) const
{
    return FVector4(X + obj.X, Y + obj.Y, Z + obj.Z, W + obj.W);
}

FVector4 FVector4::operator-(const FVector4 &obj) const
{
    return FVector4(X - obj.X, Y - obj.Y, Z - obj.Z, W - obj.W);
}

FVector4 FVector4::operator*(const FVector4 &obj) const
{
    return FVector4(X * obj.X, Y * obj.Y, Z * obj.Z, W * obj.W);
}

FVector4 FVector4::operator/(const FVector4 &obj) const
{
    return FVector4(obj.X != 0.0f ? X / obj.X : 0.0f, obj.Y != 0.0f ? Y / obj.Y : 0.0f,
                    obj.Z != 0.0f ? Z / obj.Z : 0.0f, obj.W != 0.0f ? W / obj.W : 0.0f);
}

FVector4 FVector4::operator*(const float obj) const
{
    return FVector4(X * obj, Y * obj, Z * obj, W * obj);
}

FVector4 FVector4::operator/(const float obj) const
{
    if (obj != 0.0f)
    {
        return FVector4(X / obj, Y / obj, Z / obj, W / obj);
    }
    return FVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

FVector4 &FVector4::operator+=(const FVector4 &obj)
{
    X += obj.X;
    Y += obj.Y;
    Z += obj.Z;
    W += obj.W;
    return *this;
}

FVector4 &FVector4::operator-=(const FVector4 &obj)
{
    X -= obj.X;
    Y -= obj.Y;
    Z -= obj.Z;
    W -= obj.W;
    return *this;
}

FVector4 &FVector4::operator*=(const FVector4 &obj)
{
    X *= obj.X;
    Y *= obj.Y;
    Z *= obj.Z;
    W *= obj.W;
    return *this;
}

FVector4 &FVector4::operator/=(const FVector4 &obj)
{
    if (obj.X != 0.0f)
        X /= obj.X;
    else
        X = 0.0f;
    if (obj.Y != 0.0f)
        Y /= obj.Y;
    else
        Y = 0.0f;
    if (obj.Z != 0.0f)
        Z /= obj.Z;
    else
        Z = 0.0f;
    if (obj.W != 0.0f)
        W /= obj.W;
    else
        W = 0.0f;
    return *this;
}

FVector4 &FVector4::operator*=(const float obj)
{
    X *= obj;
    Y *= obj;
    Z *= obj;
    W *= obj;
    return *this;
}

FVector4 &FVector4::operator/=(const float obj)
{
    if (obj != 0.0f)
    {
        X /= obj;
        Y /= obj;
        Z /= obj;
        W /= obj;
    }
    else
    {
        X = Y = Z = W = 0.0f;
    }
    return *this;
}

bool FVector4::operator==(const FVector4 &other) const
{
    constexpr float EPSILON = 1e-5f;
    return (std::fabs(X - other.X) < EPSILON) && (std::fabs(Y - other.Y) < EPSILON) &&
           (std::fabs(Z - other.Z) < EPSILON) && (std::fabs(W - other.W) < EPSILON);
}

bool FVector4::operator!=(const FVector4 &other) const
{
    return !(*this == other);
}

float FVector4::Length() const
{
    return std::sqrt(X * X + Y * Y + Z * Z + W * W);
}

float FVector4::LengthSquared() const
{
    return X * X + Y * Y + Z * Z + W * W;
}

FVector4 FVector4::Normalize() const
{
    const float len = Length();
    if (len > 0.0f)
    {
        return FVector4(X / len, Y / len, Z / len, W / len);
    }
    return FVector4(0.0f, 0.0f, 0.0f, 0.0f);
}

float FVector4::Dot(const FVector4 &obj) const
{
    return X * obj.X + Y * obj.Y + Z * obj.Z + W * obj.W;
}

FVector4 FVector4::Clamp(const float min, const float max) const
{
    return FVector4(std::max(min, std::min(X, max)), std::max(min, std::min(Y, max)), std::max(min, std::min(Z, max)),
                    std::max(min, std::min(W, max)));
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

IVector4::IVector4() : X(0), Y(0), Z(0), W(0)
{
}

IVector4::IVector4(const int X, const int Y, const int Z, const int W) : X(X), Y(Y), Z(Z), W(W)
{
}

IVector4::IVector4(const IVector4 &copy) : X(copy.X), Y(copy.Y), Z(copy.Z), W(copy.W)
{
}

IVector4 IVector4::operator+(const IVector4 &obj) const
{
    return IVector4(X + obj.X, Y + obj.Y, Z + obj.Z, W + obj.W);
}

IVector4 IVector4::operator-(const IVector4 &obj) const
{
    return IVector4(X - obj.X, Y - obj.Y, Z - obj.Z, W - obj.W);
}

IVector4 IVector4::operator*(const IVector4 &obj) const
{
    return IVector4(X * obj.X, Y * obj.Y, Z * obj.Z, W * obj.W);
}

IVector4 IVector4::operator/(const IVector4 &obj) const
{
    return IVector4(obj.X != 0 ? X / obj.X : 0, obj.Y != 0 ? Y / obj.Y : 0, obj.Z != 0 ? Z / obj.Z : 0,
                    obj.W != 0 ? W / obj.W : 0);
}

IVector4 IVector4::operator*(const int obj) const
{
    return IVector4(X * obj, Y * obj, Z * obj, W * obj);
}

IVector4 IVector4::operator/(const int obj) const
{
    if (obj != 0)
    {
        return IVector4(X / obj, Y / obj, Z / obj, W / obj);
    }
    return IVector4(0, 0, 0, 0);
}

IVector4 &IVector4::operator+=(const IVector4 &obj)
{
    X += obj.X;
    Y += obj.Y;
    Z += obj.Z;
    W += obj.W;
    return *this;
}

IVector4 &IVector4::operator-=(const IVector4 &obj)
{
    X -= obj.X;
    Y -= obj.Y;
    Z -= obj.Z;
    W -= obj.W;
    return *this;
}

IVector4 &IVector4::operator*=(const IVector4 &obj)
{
    X *= obj.X;
    Y *= obj.Y;
    Z *= obj.Z;
    W *= obj.W;
    return *this;
}

IVector4 &IVector4::operator/=(const IVector4 &obj)
{
    if (obj.X != 0)
        X /= obj.X;
    else
        X = 0;
    if (obj.Y != 0)
        Y /= obj.Y;
    else
        Y = 0;
    if (obj.Z != 0)
        Z /= obj.Z;
    else
        Z = 0;
    if (obj.W != 0)
        W /= obj.W;
    else
        W = 0;
    return *this;
}

IVector4 &IVector4::operator*=(const int obj)
{
    X *= obj;
    Y *= obj;
    Z *= obj;
    W *= obj;
    return *this;
}

IVector4 &IVector4::operator/=(const int obj)
{
    if (obj != 0)
    {
        X /= obj;
        Y /= obj;
        Z /= obj;
        W /= obj;
    }
    else
    {
        X = Y = Z = W = 0;
    }
    return *this;
}

bool IVector4::operator==(const IVector4 &other) const
{
    return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
}

bool IVector4::operator!=(const IVector4 &other) const
{
    return !(*this == other);
}

float IVector4::Length() const
{
    return std::sqrt(static_cast<float>(X * X + Y * Y + Z * Z + W * W));
}

float IVector4::LengthSquared() const
{
    return static_cast<float>(X * X + Y * Y + Z * Z + W * W);
}

IVector4 IVector4::Normalize() const
{
    float len = Length();
    if (len > 0.0f)
    {
        return IVector4(static_cast<int>(X / len), static_cast<int>(Y / len), static_cast<int>(Z / len),
                        static_cast<int>(W / len));
    }
    return IVector4(0, 0, 0, 0);
}

int IVector4::Dot(const IVector4 &obj) const
{
    return X * obj.X + Y * obj.Y + Z * obj.Z + W * obj.W;
}

IVector4 IVector4::Clamp(const int min, const int max) const
{
    return IVector4(std::max(min, std::min(X, max)), std::max(min, std::min(Y, max)), std::max(min, std::min(Z, max)),
                    std::max(min, std::min(W, max)));
}

IVector4 IVector4::Project(const IVector4 &normal) const
{
    const float normalLengthSquared = static_cast<float>(normal.Dot(normal));
    if (normalLengthSquared > 0.0f)
    {
        const float scalar = static_cast<float>(this->Dot(normal)) / normalLengthSquared;
        return IVector4(static_cast<int>(normal.X * scalar), static_cast<int>(normal.Y * scalar),
                        static_cast<int>(normal.Z * scalar), static_cast<int>(normal.W * scalar));
    }
    return IVector4(0, 0, 0, 0);
}

#include "stdafx.h"
#include "Vector3.h"
#include <corecrt_math.h>

FVector3::FVector3()
{
    this->X = 0.0f;
    this->Y = 0.0f;
    this->Z = 0.0f;
}

FVector3::FVector3(float X, float Y, float Z)
{
    this->X = X;
    this->Y = Y;
    this->Z = Z;
}

FVector3::FVector3(const FVector3 &copy)
{
    this->X = copy.X;
    this->Y = copy.Y;
    this->Z = copy.Z;
}

FVector3 FVector3::operator+(const FVector3 &obj) const
{
    return {this->X + obj.X, this->Y + obj.Y, this->Z + obj.Z};
}

FVector3 FVector3::operator-(const FVector3 &obj) const
{
    return {this->X - obj.X, this->Y - obj.Y, this->Z - obj.Z};
}

FVector3 FVector3::operator*(const FVector3 &obj) const
{
    return {this->X * obj.X, this->Y * obj.Y, this->Z * obj.Z};
}

FVector3 FVector3::operator/(const FVector3 &obj) const
{
    return {this->X / obj.X, this->Y / obj.Y, this->Z / obj.Z};
}

FVector3 FVector3::operator*(const float obj) const
{
    return {this->X * obj, this->Y * obj, this->Z * obj};
}

FVector3 FVector3::operator/(const float obj) const
{
    return {this->X / obj, this->Y / obj, this->Z / obj};
}

FVector3 &FVector3::operator+=(const FVector3 &obj)
{
    this->X += obj.X;
    this->Y += obj.Y;
    this->Z += obj.Z;
    return *this;
}

FVector3 &FVector3::operator-=(const FVector3 &obj)
{
    this->X -= obj.X;
    this->Y -= obj.Y;
    this->Z -= obj.Z;
    return *this;
}

FVector3 &FVector3::operator*=(const FVector3 &obj)
{
    this->X *= obj.X;
    this->Y *= obj.Y;
    this->Z *= obj.Z;
    return *this;
}

FVector3 &FVector3::operator/=(const FVector3 &obj)
{
    this->X /= obj.X;
    this->Y /= obj.Y;
    this->Z /= obj.Z;
    return *this;
}

FVector3 &FVector3::operator*=(const float obj)
{
    this->X *= obj;
    this->Y *= obj;
    this->Z *= obj;
    return *this;
}

FVector3 &FVector3::operator/=(const float obj)
{
    this->X /= obj;
    this->Y /= obj;
    this->Z /= obj;
    return *this;
}

bool FVector3::operator==(const FVector3 &other) const
{
    const float EPSILON = 1e-5f; // Define an appropriate epsilon value
    return (fabs(X - other.X) < EPSILON) && (fabs(Y - other.Y) < EPSILON) && (fabs(Z - other.Z) < EPSILON);
}

bool FVector3::operator!=(const FVector3 &obj) const
{
    return !(*this == obj);
}

float FVector3::Length() const
{
    return sqrtf(X * X + Y * Y + Z * Z);
}

float FVector3::LengthSquared() const
{
    return X * X + Y * Y + Z * Z;
}

FVector3 FVector3::Normalize()
{
    float len = Length();
    if (len != 0.0f)
    {
        return FVector3(X / len, Y / len, Z / len);
    }
    return FVector3(0.0f, 0.0f, 0.0f);
}

float FVector3::Dot(const FVector3 &obj) const
{
    return (this->X * obj.X) + (this->Y * obj.Y) + (this->Z * obj.Z);
}

FVector3 FVector3::Cross(const FVector3 &obj) const
{
    return FVector3(Y * obj.Z - Z * obj.Y, Z * obj.X - X * obj.Z, X * obj.Y - Y * obj.X);
}

IVector3::IVector3()
{
    this->X = 0;
    this->Y = 0;
    this->Z = 0;
}

IVector3::IVector3(int X, int Y, int Z)
{
    this->X = X;
    this->Y = Y;
    this->Z = Z;
}

IVector3::IVector3(const IVector3 &copy)
{
    this->X = copy.X;
    this->Y = copy.Y;
    this->Z = copy.Z;
}

IVector3 IVector3::operator+(const IVector3 &obj) const
{
    return {this->X + obj.X, this->Y + obj.Y, this->Z + obj.Z};
}

IVector3 IVector3::operator-(const IVector3 &obj) const
{
    return {this->X - obj.X, this->Y - obj.Y, this->Z - obj.Z};
}

IVector3 IVector3::operator*(const IVector3 &obj) const
{
    return {this->X * obj.X, this->Y * obj.Y, this->Z * obj.Z};
}

IVector3 IVector3::operator/(const IVector3 &obj) const
{
    return {this->X / obj.X, this->Y / obj.Y, this->Z / obj.Z};
}

IVector3 IVector3::operator*(const int obj) const
{
    return {this->X * obj, this->Y * obj, this->Z * obj};
}

IVector3 IVector3::operator/(const int obj) const
{
    return {this->X / obj, this->Y / obj, this->Z / obj};
}

IVector3 &IVector3::operator+=(const IVector3 &obj)
{
    this->X += obj.X;
    this->Y += obj.Y;
    this->Z += obj.Z;
    return *this;
}

IVector3 &IVector3::operator-=(const IVector3 &obj)
{
    this->X -= obj.X;
    this->Y -= obj.Y;
    this->Z -= obj.Z;
    return *this;
}

IVector3 &IVector3::operator*=(const IVector3 &obj)
{
    this->X *= obj.X;
    this->Y *= obj.Y;
    this->Z *= obj.Z;
    return *this;
}

IVector3 &IVector3::operator/=(const IVector3 &obj)
{
    this->X /= obj.X;
    this->Y /= obj.Y;
    this->Z /= obj.Z;
    return *this;
}

IVector3 &IVector3::operator*=(const int obj)
{
    this->X *= obj;
    this->Y *= obj;
    this->Z *= obj;
    return *this;
}

IVector3 &IVector3::operator/=(const int obj)
{
    this->X /= obj;
    this->Y /= obj;
    this->Z /= obj;
    return *this;
}

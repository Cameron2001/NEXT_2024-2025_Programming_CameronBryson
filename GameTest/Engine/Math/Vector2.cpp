#include "stdafx.h"
#include "Vector2.h"

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

FVector2 FVector2::Length()
{
    return FVector2();
}

FVector2 FVector2::LengthSquared()
{
    return FVector2();
}

FVector2 FVector2::Normalize()
{
    return FVector2();
}

FVector2 FVector2::Cross(FVector2 obj)
{
    return FVector2();
}

FVector2 FVector2::Clamp(float min, float max)
{
    return FVector2();
}

FVector2 FVector2::Project(FVector2 normal)
{
    return FVector2();
}

FVector2 FVector2::Dot(FVector2 obj)
{
    return FVector2();
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
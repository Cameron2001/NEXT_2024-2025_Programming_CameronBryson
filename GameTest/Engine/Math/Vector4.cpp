#include "stdafx.h"
#include "Vector4.h"

FVector4::FVector4()
{
	this->X = 0.0f;
	this->Y = 0.0f;
	this->Z = 0.0f;
	this->W = 0.0f;
}

FVector4::FVector4(float X, float Y, float W, float H)
{
	this->X = X;
	this->Y = Y;
	this->Z = W;
	this->W = H;
}

FVector4::FVector4(const FVector4& copy)
{
	this->X = copy.X;
	this->Y = copy.Y;
	this->Z = copy.Z;
	this->W = copy.W;
}

FVector4 FVector4::operator+(const FVector4& obj) const
{
	return { this->X + obj.X, this->Y + obj.Y, this->Z + obj.Z, this->W + obj.W };
}

FVector4 FVector4::operator-(const FVector4& obj) const
{
	return { this->X - obj.X, this->Y - obj.Y, this->Z - obj.Z, this->W - obj.W };
}

FVector4 FVector4::operator*(const FVector4& obj) const
{
	return { this->X * obj.X, this->Y * obj.Y, this->Z * obj.Z, this->W * obj.W };
}

FVector4 FVector4::operator/(const FVector4& obj) const
{
	return { this->X / obj.X, this->Y / obj.Y, this->Z / obj.Z, this->W / obj.W };
}

FVector4 FVector4::operator*(const float obj) const
{
	return { this->X * obj, this->Y - obj, this->Z - obj, this->W - obj };
}

FVector4 FVector4::operator/(const float obj) const
{
	return { this->X / obj, this->Y / obj, this->Z / obj, this->W / obj };
}

FVector4& FVector4::operator+=(const FVector4& obj)
{
	this->X += obj.X;
	this->Y += obj.Y;
	this->Z += obj.Z;
	this->W += obj.W;
	return *this;
}

FVector4& FVector4::operator-=(const FVector4& obj)
{
	this->X -= obj.X;
	this->Y -= obj.Y;
	this->Z -= obj.Z;
	this->W -= obj.W;
	return *this;
}

FVector4& FVector4::operator*=(const FVector4& obj)
{
	this->X *= obj.X;
	this->Y *= obj.Y;
	this->Z *= obj.Z;
	this->W *= obj.W;
	return *this;
}

FVector4& FVector4::operator/=(const FVector4& obj)
{
	this->X /= obj.X;
	this->Y /= obj.Y;
	this->Z /= obj.Z;
	this->W /= obj.W;
	return *this;
}

FVector4& FVector4::operator*=(const float obj)
{
	this->X *= obj;
	this->Y *= obj;
	this->Z *= obj;
	this->W *= obj;
	return *this;
}

FVector4& FVector4::operator/=(const float obj)
{
	this->X /= obj;
	this->Y /= obj;
	this->Z /= obj;
	this->W /= obj;
	return *this;
}
IVector4::IVector4()
{
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
	this->W = 0;
}

IVector4::IVector4(int X, int Y, int W, int H)
{
	this->X = X;
	this->Y = Y;
	this->Z = W;
	this->W = H;
}

IVector4::IVector4(const IVector4& copy)
{
	this->X = copy.X;
	this->Y = copy.Y;
	this->Z = copy.Z;
	this->W = copy.W;
}

IVector4 IVector4::operator+(const IVector4& obj) const
{
	return { this->X + obj.X, this->Y + obj.Y, this->Z + obj.Z, this->W + obj.W };
}

IVector4 IVector4::operator-(const IVector4& obj) const
{
	return { this->X - obj.X, this->Y - obj.Y, this->Z - obj.Z, this->W - obj.W };
}

IVector4 IVector4::operator*(const IVector4& obj) const
{
	return { this->X * obj.X, this->Y * obj.Y, this->Z * obj.Z, this->W * obj.W };
}

IVector4 IVector4::operator/(const IVector4& obj) const
{
	return { this->X / obj.X, this->Y / obj.Y, this->Z / obj.Z, this->W / obj.W };
}

IVector4 IVector4::operator*(const int obj) const
{
	return { this->X * obj, this->Y * obj, this->Z * obj, this->W * obj};
}

IVector4 IVector4::operator/(const int obj) const
{
	return { this->X / obj, this->Y / obj, this->Z / obj, this->W / obj };
}

IVector4& IVector4::operator+=(const IVector4& obj)
{
	this->X += obj.X;
	this->Y += obj.Y;
	this->Z += obj.Z;
	this->W += obj.W;
	return *this;
}

IVector4& IVector4::operator-=(const IVector4& obj)
{
	this->X -= obj.X;
	this->Y -= obj.Y;
	this->Z -= obj.Z;
	this->W -= obj.W;
	return *this;
}

IVector4& IVector4::operator*=(const IVector4& obj)
{
	this->X *= obj.X;
	this->Y *= obj.Y;
	this->Z *= obj.Z;
	this->W *= obj.W;
	return *this;
}

IVector4& IVector4::operator/=(const IVector4& obj)
{
	this->X /= obj.X;
	this->Y /= obj.Y;
	this->Z /= obj.Z;
	this->W /= obj.W;
	return *this;
}

IVector4& IVector4::operator*=(const int obj)
{
	this->X *= obj;
	this->Y *= obj;
	this->Z *= obj;
	this->W *= obj;
	return *this;
}

IVector4& IVector4::operator/=(const int obj)
{
	this->X /= obj;
	this->Y /= obj;
	this->Z /= obj;
	this->W /= obj;
	return *this;
}

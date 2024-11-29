#include "stdafx.h"
#include "Vector4.h"

FVector4::FVector4()
{
	this->X = 0.0f;
	this->Y = 0.0f;
	this->W = 0.0f;
	this->H = 0.0f;
}

FVector4::FVector4(float X, float Y, float W, float H)
{
	this->X = X;
	this->Y = Y;
	this->W = W;
	this->H = H;
}

FVector4::FVector4(const FVector4& copy)
{
	this->X = copy.X;
	this->Y = copy.Y;
	this->W = copy.W;
	this->H = copy.H;
}

FVector4 FVector4::operator+(const FVector4& obj) const
{
	return { this->X + obj.X, this->Y + obj.Y, this->W + obj.W, this->H + obj.H };
}

FVector4 FVector4::operator-(const FVector4& obj) const
{
	return { this->X - obj.X, this->Y - obj.Y, this->W - obj.W, this->H - obj.H };
}

FVector4 FVector4::operator*(const FVector4& obj) const
{
	return { this->X * obj.X, this->Y * obj.Y, this->W * obj.W, this->H * obj.H };
}

FVector4 FVector4::operator/(const FVector4& obj) const
{
	return { this->X / obj.X, this->Y / obj.Y, this->W / obj.W, this->H / obj.H };
}

FVector4 FVector4::operator*(const float obj) const
{
	return { this->X * obj, this->Y - obj, this->W - obj, this->H - obj };
}

FVector4 FVector4::operator/(const float obj) const
{
	return { this->X / obj, this->Y / obj, this->W / obj, this->H / obj };
}

FVector4& FVector4::operator+=(const FVector4& obj)
{
	this->X += obj.X;
	this->Y += obj.Y;
	this->W += obj.W;
	this->H += obj.H;
	return *this;
}

FVector4& FVector4::operator-=(const FVector4& obj)
{
	this->X -= obj.X;
	this->Y -= obj.Y;
	this->W -= obj.W;
	this->H -= obj.H;
	return *this;
}

FVector4& FVector4::operator*=(const FVector4& obj)
{
	this->X *= obj.X;
	this->Y *= obj.Y;
	this->W *= obj.W;
	this->H *= obj.H;
	return *this;
}

FVector4& FVector4::operator/=(const FVector4& obj)
{
	this->X /= obj.X;
	this->Y /= obj.Y;
	this->W /= obj.W;
	this->H /= obj.H;
	return *this;
}

FVector4& FVector4::operator*=(const float obj)
{
	this->X *= obj;
	this->Y *= obj;
	this->W *= obj;
	this->H *= obj;
	return *this;
}

FVector4& FVector4::operator/=(const float obj)
{
	this->X /= obj;
	this->Y /= obj;
	this->W /= obj;
	this->H /= obj;
	return *this;
}
IVector4::IVector4()
{
	this->X = 0;
	this->Y = 0;
	this->W = 0;
	this->H = 0;
}

IVector4::IVector4(int X, int Y, int W, int H)
{
	this->X = X;
	this->Y = Y;
	this->W = W;
	this->H = H;
}

IVector4::IVector4(const IVector4& copy)
{
	this->X = copy.X;
	this->Y = copy.Y;
	this->W = copy.W;
	this->H = copy.H;
}

IVector4 IVector4::operator+(const IVector4& obj) const
{
	return { this->X + obj.X, this->Y + obj.Y, this->W + obj.W, this->H + obj.H };
}

IVector4 IVector4::operator-(const IVector4& obj) const
{
	return { this->X - obj.X, this->Y - obj.Y, this->W - obj.W, this->H - obj.H };
}

IVector4 IVector4::operator*(const IVector4& obj) const
{
	return { this->X * obj.X, this->Y * obj.Y, this->W * obj.W, this->H * obj.H };
}

IVector4 IVector4::operator/(const IVector4& obj) const
{
	return { this->X / obj.X, this->Y / obj.Y, this->W / obj.W, this->H / obj.H };
}

IVector4 IVector4::operator*(const int obj) const
{
	return { this->X * obj, this->Y * obj, this->W * obj, this->H * obj};
}

IVector4 IVector4::operator/(const int obj) const
{
	return { this->X / obj, this->Y / obj, this->W / obj, this->H / obj };
}

IVector4& IVector4::operator+=(const IVector4& obj)
{
	this->X += obj.X;
	this->Y += obj.Y;
	this->W += obj.W;
	this->H += obj.H;
	return *this;
}

IVector4& IVector4::operator-=(const IVector4& obj)
{
	this->X -= obj.X;
	this->Y -= obj.Y;
	this->W -= obj.W;
	this->H -= obj.H;
	return *this;
}

IVector4& IVector4::operator*=(const IVector4& obj)
{
	this->X *= obj.X;
	this->Y *= obj.Y;
	this->W *= obj.W;
	this->H *= obj.H;
	return *this;
}

IVector4& IVector4::operator/=(const IVector4& obj)
{
	this->X /= obj.X;
	this->Y /= obj.Y;
	this->W /= obj.W;
	this->H /= obj.H;
	return *this;
}

IVector4& IVector4::operator*=(const int obj)
{
	this->X *= obj;
	this->Y *= obj;
	this->W *= obj;
	this->H *= obj;
	return *this;
}

IVector4& IVector4::operator/=(const int obj)
{
	this->X /= obj;
	this->Y /= obj;
	this->W /= obj;
	this->H /= obj;
	return *this;
}

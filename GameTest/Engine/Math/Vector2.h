#pragma once
class FVector2
{
public:
	float X;
	float Y;
public:
	FVector2();
	FVector2(float X, float Y);
	FVector2(const FVector2& copy);
public:
	FVector2 Length();
	FVector2 LengthSquared();
	FVector2 Normalize();
	FVector2 Cross(FVector2 obj);
	FVector2 Clamp(float min, float max);
	FVector2 Project(FVector2 normal);
	FVector2 Dot(FVector2 obj);
public:
	FVector2 operator+(const FVector2& obj) const;
	FVector2 operator-(const FVector2& obj) const;
	FVector2 operator*(const FVector2& obj) const;
	FVector2 operator/(const FVector2& obj) const;

	FVector2 operator*(const float obj) const;
	FVector2 operator/(const float obj) const;
public:
	FVector2& operator+=(const FVector2& obj);
	FVector2& operator-=(const FVector2& obj);
	FVector2& operator*=(const FVector2& obj);
	FVector2& operator/=(const FVector2& obj);

	FVector2& operator*=(const float obj);
	FVector2& operator/=(const float obj);

};
class IVector2 
{
public:
	int X;
	int Y;
public:
	IVector2();
	IVector2(int X, int Y);
	IVector2(const IVector2& copy);
public:
	IVector2 Length();
	IVector2 LengthSquared();
	IVector2 Normalize();
	IVector2 Cross(IVector2 obj);
	IVector2 Clamp(int min, int max);
	IVector2 Project(IVector2 normal);
	IVector2 Dot(IVector2 obj);
public:
	IVector2 operator+(const IVector2& obj) const;
	IVector2 operator-(const IVector2& obj) const;
	IVector2 operator*(const IVector2& obj) const;
	IVector2 operator/(const IVector2& obj) const;

	IVector2 operator*(const int obj) const;
	IVector2 operator/(const int obj) const;
public:
	IVector2& operator+=(const IVector2& obj);
	IVector2& operator-=(const IVector2& obj);
	IVector2& operator*=(const IVector2& obj);
	IVector2& operator/=(const IVector2& obj);

	IVector2& operator*=(const int obj);
	IVector2& operator/=(const int obj);
};


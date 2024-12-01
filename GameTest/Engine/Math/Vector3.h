#pragma once
class FVector3
{
public:
	float X;
	float Y;
	float Z;
public:
	FVector3();
	FVector3(float X, float Y, float Z);
	FVector3(const FVector3& copy);
public:
	FVector3 operator+(const FVector3& obj) const;
	FVector3 operator-(const FVector3& obj) const;
	FVector3 operator*(const FVector3& obj) const;
	FVector3 operator/(const FVector3& obj) const;

	FVector3 operator*(const float obj) const;
	FVector3 operator/(const float obj) const;
public:
	FVector3& operator+=(const FVector3& obj);
	FVector3& operator-=(const FVector3& obj);
	FVector3& operator*=(const FVector3& obj);
	FVector3& operator/=(const FVector3& obj);

	FVector3& operator*=(const float obj);
	FVector3& operator/=(const float obj);
public:
	float Length() const;
	float LengthSquared() const;
	FVector3 Normalize();
	float Dot(const FVector3& obj) const;
	FVector3 Cross(const FVector3& obj) const;
};
class IVector3 {
public:
	int X;
	int Y;
	int Z;
public:
	IVector3();
	IVector3(int X, int Y, int Z);
	IVector3(const IVector3& copy);
public:
	IVector3 operator+(const IVector3& obj) const;
	IVector3 operator-(const IVector3& obj) const;
	IVector3 operator*(const IVector3& obj) const;
	IVector3 operator/(const IVector3& obj) const;

	IVector3 operator*(const int obj) const;
	IVector3 operator/(const int obj) const;
public:
	IVector3& operator+=(const IVector3& obj);
	IVector3& operator-=(const IVector3& obj);
	IVector3& operator*=(const IVector3& obj);
	IVector3& operator/=(const IVector3& obj);

	IVector3& operator*=(const int obj);
	IVector3& operator/=(const int obj);
};


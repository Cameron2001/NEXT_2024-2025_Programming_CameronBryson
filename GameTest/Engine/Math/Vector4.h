#pragma once
class FVector4
{
public:
	float X;
	float Y;
	float W;
	float H;
public:
	FVector4();
	FVector4(float X, float Y, float W, float H);
	FVector4(const FVector4& copy);
public:
	FVector4 operator+(const FVector4& obj) const;
	FVector4 operator-(const FVector4& obj) const;
	FVector4 operator*(const FVector4& obj) const;
	FVector4 operator/(const FVector4& obj) const;

	FVector4 operator*(const float obj) const;
	FVector4 operator/(const float obj) const;
public:
	FVector4& operator+=(const FVector4& obj);
	FVector4& operator-=(const FVector4& obj);
	FVector4& operator*=(const FVector4& obj);
	FVector4& operator/=(const FVector4& obj);

	FVector4& operator*=(const float obj);
	FVector4& operator/=(const float obj);
};
class IVector4 {
public:
	int X;
	int Y;
	int W;
	int H;
public:
	IVector4();
	IVector4(int X, int Y, int W, int H);
	IVector4(const IVector4& copy);
public:
	IVector4 operator+(const IVector4& obj) const;
	IVector4 operator-(const IVector4& obj) const;
	IVector4 operator*(const IVector4& obj) const;
	IVector4 operator/(const IVector4& obj) const;

	IVector4 operator*(const int obj) const;
	IVector4 operator/(const int obj) const;
public:
	IVector4& operator+=(const IVector4& obj);
	IVector4& operator-=(const IVector4& obj);
	IVector4& operator*=(const IVector4& obj);
	IVector4& operator/=(const IVector4& obj);

	IVector4& operator*=(const int obj);
	IVector4& operator/=(const int obj);
};



#pragma once
class FVector4
{
public:
    float X;
    float Y;
    float Z;
    float W;
    FVector4();
    FVector4(float X, float Y, float W, float H);
    FVector4(const FVector4 &copy);
    FVector4 operator+(const FVector4 &obj) const;
    FVector4 operator-(const FVector4 &obj) const;
    FVector4 operator*(const FVector4 &obj) const;
    FVector4 operator/(const FVector4 &obj) const;

    FVector4 operator*(float obj) const;
    FVector4 operator/(float obj) const;
    FVector4 &operator+=(const FVector4 &obj);
    FVector4 &operator-=(const FVector4 &obj);
    FVector4 &operator*=(const FVector4 &obj);
    FVector4 &operator/=(const FVector4 &obj);

    FVector4 &operator*=(float obj);
    FVector4 &operator/=(float obj);
};

class IVector4
{
public:
    int X;
    int Y;
    int Z;
    int W;
    IVector4();
    IVector4(int X, int Y, int W, int H);
    IVector4(const IVector4 &copy);
    IVector4 operator+(const IVector4 &obj) const;
    IVector4 operator-(const IVector4 &obj) const;
    IVector4 operator*(const IVector4 &obj) const;
    IVector4 operator/(const IVector4 &obj) const;

    IVector4 operator*(int obj) const;
    IVector4 operator/(int obj) const;
    IVector4 &operator+=(const IVector4 &obj);
    IVector4 &operator-=(const IVector4 &obj);
    IVector4 &operator*=(const IVector4 &obj);
    IVector4 &operator/=(const IVector4 &obj);

    IVector4 &operator*=(int obj);
    IVector4 &operator/=(int obj);
};
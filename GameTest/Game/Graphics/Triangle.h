#pragma once
#include "Game/Math/Vector2.h"
#include "Game/Math/Vector3.h"
#include <functional>
struct Triangle2D
{


    Triangle2D() : color({1.0f, 1.0f, 1.0f}), avgZ(0.0f)
    {
    }

    Triangle2D(const FVector2 &v0, const FVector2 &v1, const FVector2 &v2, const float avgZ)
        : color({1.0f, 1.0f, 1.0f}), v0(v0), v1(v1), v2(v2), avgZ(avgZ)
    {
    }

    Triangle2D(const FVector2 &v0, const FVector2 &v1, const FVector2 &v2, const float avgZ, const FVector3 &color)
        : color(color), v0(v0), v1(v1), v2(v2), avgZ(avgZ)
    {
    }

    bool operator==(const Triangle2D &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }


    FVector3 color;
    FVector2 v0;
    FVector2 v1;
    FVector2 v2;
    float avgZ;
};
struct Triangle3D
{
    Triangle3D(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2) : v0(v0), v1(v1), v2(v2)
    {
    }
    bool operator==(const Triangle3D &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }
    FVector3 v0;
    FVector3 v1;
    FVector3 v2;
};

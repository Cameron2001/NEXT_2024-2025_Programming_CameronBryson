#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include <functional>
struct Triangle2D
{
    Triangle2D(const FVector2 &v0, const FVector2 &v1, const FVector2 &v2, float avgZ)
        : v0(v0), v1(v1), v2(v2), avgZ(avgZ)
    {
    }
    Triangle2D(const Triangle2D &other) noexcept = default;
    Triangle2D &operator=(const Triangle2D &other) noexcept = default;

    Triangle2D(Triangle2D &&other) noexcept = default;
    Triangle2D &operator=(Triangle2D &&other) noexcept = default;
    bool operator==(const Triangle2D &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }
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
    Triangle3D(const Triangle3D &other) noexcept = default;
    Triangle3D &operator=(const Triangle3D &other) noexcept = default;

    Triangle3D(Triangle3D &&other) noexcept = default;
    Triangle3D &operator=(Triangle3D &&other) noexcept = default;
    bool operator==(const Triangle3D &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }
    FVector3 v0;
    FVector3 v1;
    FVector3 v2;
};

#pragma once
#include "Game/Math/Vector2.h"
#include "Game/Math/Vector3.h"
#include <functional>
struct Triangle2D
{
    FVector2 v0;
    FVector2 v1;
    FVector2 v2;
    float avgZ;
    FVector3 color;
    int layer;

    Triangle2D() : v0(), v1(), v2(), avgZ(0.0f), color({1.0f, 1.0f, 1.0f}), layer(0)
    {
    }

    Triangle2D(const FVector2 &v0, const FVector2 &v1, const FVector2 &v2, float avgZ)
        : v0(v0), v1(v1), v2(v2), avgZ(avgZ), color({1.0f, 1.0f, 1.0f}), layer(0)
    {
    }

    Triangle2D(const FVector2 &v0, const FVector2 &v1, const FVector2 &v2, float avgZ, const FVector3 &color, int layer)
        : v0(v0), v1(v1), v2(v2), avgZ(avgZ), color(color), layer(layer)
    {
    }

    bool operator==(const Triangle2D &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }

    Triangle2D(const Triangle2D &other) noexcept = default;
    Triangle2D &operator=(const Triangle2D &other) noexcept = default;

    Triangle2D(Triangle2D &&other) noexcept = default;
    Triangle2D &operator=(Triangle2D &&other) noexcept = default;
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

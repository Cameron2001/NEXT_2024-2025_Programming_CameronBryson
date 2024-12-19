#pragma once
#include "Engine/Math/Vector3.h"
#include <functional>
struct Triangle
{
    Triangle(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2) : v0(v0), v1(v1), v2(v2)
    {
        isFrontFacing = false;
    }
    Triangle(const Triangle &other, bool isFrontFacing)
        : v0(other.v0), v1(other.v1), v2(other.v2), isFrontFacing(isFrontFacing)
    {
    }
    bool operator==(const Triangle &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }
    FVector3 v0;
    FVector3 v1;
    FVector3 v2;
    mutable bool isFrontFacing;
};
struct TriangleHasher
{
    std::size_t operator()(const Triangle &triangle) const
    {
        std::size_t h1 = std::hash<float>()(triangle.v0.X) ^ std::hash<float>()(triangle.v0.Y) << 1;
        std::size_t h2 = std::hash<float>()(triangle.v1.X) ^ std::hash<float>()(triangle.v1.Y) << 1;
        std::size_t h3 = std::hash<float>()(triangle.v2.X) ^ std::hash<float>()(triangle.v2.Y) << 1;
        return h1 ^ h2 ^ h3;
    }
};

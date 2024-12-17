#pragma once
#include "Engine/Math/Vector3.h"
#include <functional>
struct Face
{
    Face(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2, const FVector3 &normal)
        : v0(v0), v1(v1), v2(v2), normal(normal)
    {
    }
    Face(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2) : v0(v0), v1(v1), v2(v2)
    {
        normal = {0,0,0};
    }
    bool operator==(const Face &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }
    FVector3 v0;
    FVector3 v1;
    FVector3 v2;
    FVector3 normal;
};
struct FaceHash
{
    std::size_t operator()(const Face &face) const
    {
        std::size_t h1 = std::hash<float>()(face.v0.X) ^ std::hash<float>()(face.v0.Y) << 1;
        std::size_t h2 = std::hash<float>()(face.v1.X) ^ std::hash<float>()(face.v1.Y) << 1;
        std::size_t h3 = std::hash<float>()(face.v2.X) ^ std::hash<float>()(face.v2.Y) << 1;
        return h1 ^ h2 ^ h3;
    }
};
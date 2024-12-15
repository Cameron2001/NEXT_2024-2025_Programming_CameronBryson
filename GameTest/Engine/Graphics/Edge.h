#pragma once
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <functional>
struct Edge2D
{
    Edge2D(const FVector2 &start2D, const FVector2 &end2D) : start(start2D), end(end2D)
    {
    }
    FVector2 start;
    FVector2 end;
    bool operator==(const Edge2D &other) const
    {
        return start == other.start && end == other.end;
    };
    Edge2D &operator=(const Edge2D &other)
    {
        start = other.start;
        end = other.end;
        return *this;
    }
};
struct Edge3D
{
    Edge3D() = default;
    Edge3D(const FVector3 &start3D, const FVector3 &end3D) : start(start3D), end(end3D)
    {
    }
    FVector3 start;
    FVector3 end;
    bool operator==(const Edge3D &other) const
    {
        // Check for equality regardless of the order of start and end points
        return ((start == other.start && end == other.end) || (start == other.end && end == other.start));
    }
    Edge3D &operator=(const Edge3D &other)
    {
        start = other.start;
        end = other.end;
        return *this;
    }
};
namespace std
{
template <> struct hash<Edge3D>
{
    size_t operator()(const Edge3D &edge) const
    {
        size_t h1 = hash<float>()(edge.start.X) ^ hash<float>()(edge.start.Y) ^ hash<float>()(edge.start.Z);
        size_t h2 = hash<float>()(edge.end.X) ^ hash<float>()(edge.end.Y) ^ hash<float>()(edge.end.Z);
        return h1 ^ h2;
    }
};
} // namespace std

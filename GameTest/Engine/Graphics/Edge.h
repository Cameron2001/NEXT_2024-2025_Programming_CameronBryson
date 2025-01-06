#pragma once
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <functional>

struct Edge2D
{
    Edge2D() = default;

    Edge2D(const FVector2 &start2D, const FVector2 &end2D) : start(start2D), end(end2D)
    {
    }

    bool operator==(const Edge2D &other) const
    {
        return (start == other.start && end == other.end) || (start == other.end && end == other.start);
    }
    bool operator<(const Edge2D &other) const
    {
        if (start.X != other.start.X)
            return start.X < other.start.X;
        if (start.Y != other.start.Y)
            return start.Y < other.start.Y;
        if (end.X != other.end.X)
            return end.X < other.end.X;
        return end.Y < other.end.Y;
    }
    Edge2D(const Edge2D &other) = default;

    Edge2D &operator=(const Edge2D &other) = default;

    Edge2D(Edge2D &&other) noexcept = default;

    Edge2D &operator=(Edge2D &&other) noexcept = default;

    FVector2 start;
    FVector2 end;
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
        return (start == other.start && end == other.end) || (start == other.end && end == other.start);
    }

    Edge3D(const Edge3D &other) = default;
    Edge3D &operator=(const Edge3D &other) = default;
    Edge3D(Edge3D &&other) noexcept = default;
    Edge3D &operator=(Edge3D &&other) noexcept = default;
};


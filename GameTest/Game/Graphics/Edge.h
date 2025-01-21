#pragma once
#include <Game/Math/Vector2.h>
#include <Game/Math/Vector3.h>
#include <functional>

struct Edge2D
{


    Edge2D() = default;

    Edge2D(const FVector2 &start2D, const FVector2 &end2D)
        : start(start2D), end(end2D), color({1.0f, 1.0f, 1.0f})
    {
    }

    Edge2D(const FVector2 &start2D, const FVector2 &end2D, const FVector3 &color)
        : start(start2D), end(end2D), color(color)
    {
    }

    bool operator==(const Edge2D &other) const
    {
        return (start == other.start && end == other.end) || (start == other.end && end == other.start);
    }

    bool operator<(const Edge2D &other) const
    {
        if (start.x != other.start.x)
            return start.x < other.start.x;
        if (start.y != other.start.y)
            return start.y < other.start.y;
        if (end.x != other.end.x)
            return end.x < other.end.x;
        return end.y < other.end.y;
    }
    FVector2 start;
    FVector2 end;
    FVector3 color;
};

struct Edge3D
{

    Edge3D() = default;

    Edge3D(const FVector3 &start3D, const FVector3 &end3D)
        : start(start3D), end(end3D), color({1.0f, 1.0f, 1.0f})
    {
    }

    Edge3D(const FVector3 &start3D, const FVector3 &end3D, const FVector3 &color)
        : start(start3D), end(end3D), color(color)
    {
    }

    bool operator==(const Edge3D &other) const
    {
        return (start == other.start && end == other.end) || (start == other.end && end == other.start);
    }

    FVector3 start;
    FVector3 end;
    FVector3 color;
};

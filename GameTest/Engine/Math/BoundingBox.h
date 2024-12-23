#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include <algorithm>

class BoundingBox2D
{
  public:
    float minX, minY;
    float maxX, maxY;

    BoundingBox2D() : minX(0), minY(0), maxX(0), maxY(0)
    {
    }

    BoundingBox2D(const float minX_, const float minY_, const float maxX_, const float maxY_)
        : minX(minX_), minY(minY_), maxX(maxX_), maxY(maxY_)
    {
    }

    BoundingBox2D(const FVector2 &minPoint, const FVector2 &maxPoint)
        : minX(minPoint.X), minY(minPoint.Y), maxX(maxPoint.X), maxY(maxPoint.Y)
    {
    }

    bool intersects(const BoundingBox2D &other) const
    {
        return !(maxX < other.minX || minX > other.maxX || maxY < other.minY || minY > other.maxY);
    }

    bool contains(const BoundingBox2D &other) const
    {
        return (minX <= other.minX && maxX >= other.maxX && minY <= other.minY && maxY >= other.maxY);
    }

    bool containsPoint(const FVector2 &point) const
    {
        return (point.X >= minX && point.X <= maxX && point.Y >= minY && point.Y <= maxY);
    }
};

class BoundingBox3D
{
  public:
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    BoundingBox3D() : minX(0), minY(0), minZ(0), maxX(0), maxY(0), maxZ(0)
    {
    }

    BoundingBox3D(const float minX_, const float minY_, const float minZ_, const float maxX_, const float maxY_, float max_z)
        : minX(minX_), minY(minY_), minZ(minZ_), maxX(maxX_), maxY(maxY_), maxZ(max_z)
    {
    }

    BoundingBox3D(const FVector3 &minPoint, const FVector3 &maxPoint)
        : minX(minPoint.X), minY(minPoint.Y), minZ(minPoint.Z), maxX(maxPoint.X), maxY(maxPoint.Y), maxZ(maxPoint.Z)
    {
    }

    bool intersects(const BoundingBox3D &other) const
    {
        return !(maxX < other.minX || minX > other.maxX || maxY < other.minY || minY > other.maxY ||
                 maxZ < other.minZ || minZ > other.maxZ);
    }

    bool contains(const BoundingBox3D &other) const
    {
        return (minX <= other.minX && maxX >= other.maxX && minY <= other.minY && maxY >= other.maxY &&
                minZ <= other.minZ && maxZ >= other.maxZ);
    }

    bool containsPoint(const FVector3 &point) const
    {
        return (point.X >= minX && point.X <= maxX && point.Y >= minY && point.Y <= maxY && point.Z >= minZ &&
                point.Z <= maxZ);
    }
};

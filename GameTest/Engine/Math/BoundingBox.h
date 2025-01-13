#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "BoundingSphere.h"
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
        : minX(minPoint.x), minY(minPoint.y), maxX(maxPoint.x), maxY(maxPoint.y)
    {
    }

    bool Intersects(const BoundingBox2D &other) const
    {
        return !(maxX < other.minX || minX > other.maxX || maxY < other.minY || minY > other.maxY);
    }

    bool Contains(const BoundingBox2D &other) const
    {
        return (minX <= other.minX && maxX >= other.maxX && minY <= other.minY && maxY >= other.maxY);
    }

    bool ContainsPoint(const FVector2 &point) const
    {
        return (point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY);
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

    BoundingBox3D(const float minX_, const float minY_, const float minZ_, const float maxX_, const float maxY_,
                  const float max_z)
        : minX(minX_), minY(minY_), minZ(minZ_), maxX(maxX_), maxY(maxY_), maxZ(max_z)
    {
    }

    BoundingBox3D(const FVector3 &minPoint, const FVector3 &maxPoint)
        : minX(minPoint.x), minY(minPoint.y), minZ(minPoint.z), maxX(maxPoint.x), maxY(maxPoint.y), maxZ(maxPoint.z)
    {
    }

    bool Intersects(const BoundingBox3D &other) const
    {
        return !(maxX < other.minX || minX > other.maxX || maxY < other.minY || minY > other.maxY ||
                 maxZ < other.minZ || minZ > other.maxZ);
    }

    bool Contains(const BoundingBox3D &other) const
    {
        return (minX <= other.minX && maxX >= other.maxX && minY <= other.minY && maxY >= other.maxY &&
                minZ <= other.minZ && maxZ >= other.maxZ);
    }

    bool ContainsPoint(const FVector3 &point) const
    {
        return (point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY && point.z >= minZ &&
                point.z <= maxZ);
    }
    bool IntersectsSphere(const BoundingSphere3D &sphere) const
    {
        float sqDist = 0.0f;

        if (sphere.Center.x < minX)
            sqDist += (minX - sphere.Center.x) * (minX - sphere.Center.x);
        else if (sphere.Center.x > maxX)
            sqDist += (sphere.Center.x - maxX) * (sphere.Center.x - maxX);

        if (sphere.Center.y < minY)
            sqDist += (minY - sphere.Center.y) * (minY - sphere.Center.y);
        else if (sphere.Center.y > maxY)
            sqDist += (sphere.Center.y - maxY) * (sphere.Center.y - maxY);

        if (sphere.Center.z < minZ)
            sqDist += (minZ - sphere.Center.z) * (minZ - sphere.Center.z);
        else if (sphere.Center.z > maxZ)
            sqDist += (sphere.Center.z - maxZ) * (sphere.Center.z - maxZ);

        return sqDist <= (sphere.Radius * sphere.Radius);
    }
};

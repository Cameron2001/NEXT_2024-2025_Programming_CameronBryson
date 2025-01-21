#pragma once
#include "BoundingSphere.h"
#include "Game/Math/Vector2.h"
#include "Game/Math/Vector3.h"
#include <Game/Core/Components.h>
#include <vector>
#include <algorithm>
#include <cmath>

class BoundingBox2D
{
  public:


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
    float minX, minY;
    float maxX, maxY;
};

class BoundingBox3D
{
  public:


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
    static BoundingBox3D CreateTransformedBox(const BoxBoundsComponent &box, const TransformComponent &transform)
    {
        // Define the 8 corners of the local box
        std::vector<FVector3> localCorners = {FVector3(-box.extents.x, -box.extents.y, -box.extents.z),
                                              FVector3(box.extents.x, -box.extents.y, -box.extents.z),
                                              FVector3(-box.extents.x, box.extents.y, -box.extents.z),
                                              FVector3(box.extents.x, box.extents.y, -box.extents.z),
                                              FVector3(-box.extents.x, -box.extents.y, box.extents.z),
                                              FVector3(box.extents.x, -box.extents.y, box.extents.z),
                                              FVector3(-box.extents.x, box.extents.y, box.extents.z),
                                              FVector3(box.extents.x, box.extents.y, box.extents.z)};

        // Initialize minimum and maximum points with the first transformed corner
        FVector3 firstTransformed = transform.rotation.RotateVector3(FVector3(localCorners[0].x * transform.scale.x,
                                                                              localCorners[0].y * transform.scale.y,
                                                                              localCorners[0].z * transform.scale.z)) +
                                    transform.position;

        FVector3 minPoint = firstTransformed;
        FVector3 maxPoint = firstTransformed;

        // Iterate through all corners, apply scale, rotation, and translation
        for (size_t i = 1; i < localCorners.size(); ++i)
        {
            FVector3 scaled = FVector3(localCorners[i].x * transform.scale.x, localCorners[i].y * transform.scale.y,
                                       localCorners[i].z * transform.scale.z);

            FVector3 rotated = transform.rotation.RotateVector3(scaled);
            FVector3 worldPos = rotated + transform.position;

            minPoint.x = (std::min)(minPoint.x, worldPos.x);
            minPoint.y = (std::min)(minPoint.y, worldPos.y);
            minPoint.z = (std::min)(minPoint.z, worldPos.z);

            maxPoint.x = (std::max)(maxPoint.x, worldPos.x);
            maxPoint.y = (std::max)(maxPoint.y, worldPos.y);
            maxPoint.z = (std::max)(maxPoint.z, worldPos.z);
        }

        return BoundingBox3D(minPoint, maxPoint);
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

        if (sphere.center.x < minX)
            sqDist += (minX - sphere.center.x) * (minX - sphere.center.x);
        else if (sphere.center.x > maxX)
            sqDist += (sphere.center.x - maxX) * (sphere.center.x - maxX);

        if (sphere.center.y < minY)
            sqDist += (minY - sphere.center.y) * (minY - sphere.center.y);
        else if (sphere.center.y > maxY)
            sqDist += (sphere.center.y - maxY) * (sphere.center.y - maxY);

        if (sphere.center.z < minZ)
            sqDist += (minZ - sphere.center.z) * (minZ - sphere.center.z);
        else if (sphere.center.z > maxZ)
            sqDist += (sphere.center.z - maxZ) * (sphere.center.z - maxZ);

        return sqDist <= (sphere.radius * sphere.radius);
    }
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
};

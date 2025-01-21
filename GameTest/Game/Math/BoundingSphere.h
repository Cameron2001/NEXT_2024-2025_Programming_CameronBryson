#pragma once
#include "Game/Math/Vector2.h"
#include "Game/Math/Vector3.h"
#include <algorithm>

class BoundingSphere2D
{
  public:


    BoundingSphere2D() : center(FVector2(0.0f, 0.0f)), radius(0.0f)
    {
    }

    BoundingSphere2D(const FVector2 &center, float radius) : center(center), radius(radius)
    {
    }

    BoundingSphere2D(float centerX, float centerY, float radius) : center(FVector2(centerX, centerY)), radius(radius)
    {
    }

    bool Intersects(const BoundingSphere2D &other) const
    {
        float distanceSquared = (center - other.center).LengthSquared();
        float radiusSum = radius + other.radius;
        return distanceSquared <= (radiusSum * radiusSum);
    }

    bool Contains(const BoundingSphere2D &other) const
    {
        float distance = (center - other.center).Length();
        return distance + other.radius <= radius;
    }

    bool ContainsPoint(const FVector2 &point) const
    {
        float distanceSquared = (point - center).LengthSquared();
        return distanceSquared <= (radius * radius);
    }
    FVector2 center;
    float radius;
};

class BoundingSphere3D
{
  public:


    BoundingSphere3D() : center(FVector3(0.0f, 0.0f, 0.0f)), radius(0.0f)
    {
    }

    BoundingSphere3D(const FVector3 &center, float radius) : center(center), radius(radius)
    {
    }

    BoundingSphere3D(float centerX, float centerY, float centerZ, float radius)
        : center(FVector3(centerX, centerY, centerZ)), radius(radius)
    {
    }

    // Check if this sphere intersects with another BoundingSphere3D
    bool Intersects(const BoundingSphere3D &other) const
    {
        float distanceSquared = (center - other.center).LengthSquared();
        float radiusSum = radius + other.radius;
        return distanceSquared <= (radiusSum * radiusSum);
    }

    // Check if this sphere contains another BoundingSphere3D
    bool Contains(const BoundingSphere3D &other) const
    {
        float distance = (center - other.center).Length();
        return distance + other.radius <= radius;
    }

    // Check if this sphere contains a point
    bool ContainsPoint(const FVector3 &point) const
    {
        float distanceSquared = (point - center).LengthSquared();
        return distanceSquared <= (radius * radius);
    }
    FVector3 center;
    float radius;
};

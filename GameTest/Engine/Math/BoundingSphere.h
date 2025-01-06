#pragma once

#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include <algorithm>

class BoundingSphere2D
{
  public:
    FVector2 Center;
    float Radius;

    BoundingSphere2D() : Center(FVector2(0.0f, 0.0f)), Radius(0.0f)
    {
    }

    BoundingSphere2D(const FVector2 &center, float radius) : Center(center), Radius(radius)
    {
    }

    BoundingSphere2D(float centerX, float centerY, float radius) : Center(FVector2(centerX, centerY)), Radius(radius)
    {
    }

    bool Intersects(const BoundingSphere2D &other) const
    {
        float distanceSquared = (Center - other.Center).LengthSquared();
        float radiusSum = Radius + other.Radius;
        return distanceSquared <= (radiusSum * radiusSum);
    }

    bool Contains(const BoundingSphere2D &other) const
    {
        float distance = (Center - other.Center).Length();
        return distance + other.Radius <= Radius;
    }

    bool ContainsPoint(const FVector2 &point) const
    {
        float distanceSquared = (point - Center).LengthSquared();
        return distanceSquared <= (Radius * Radius);
    }
};

class BoundingSphere3D
{
  public:
    FVector3 Center;
    float Radius;

    BoundingSphere3D() : Center(FVector3(0.0f, 0.0f, 0.0f)), Radius(0.0f)
    {
    }

    BoundingSphere3D(const FVector3 &center, float radius) : Center(center), Radius(radius)
    {
    }

    BoundingSphere3D(float centerX, float centerY, float centerZ, float radius)
        : Center(FVector3(centerX, centerY, centerZ)), Radius(radius)
    {
    }

    // Check if this sphere intersects with another BoundingSphere3D
    bool Intersects(const BoundingSphere3D &other) const
    {
        float distanceSquared = (Center - other.Center).LengthSquared();
        float radiusSum = Radius + other.Radius;
        return distanceSquared <= (radiusSum * radiusSum);
    }

    // Check if this sphere contains another BoundingSphere3D
    bool Contains(const BoundingSphere3D &other) const
    {
        float distance = (Center - other.Center).Length();
        return distance + other.Radius <= Radius;
    }

    // Check if this sphere contains a point
    bool ContainsPoint(const FVector3 &point) const
    {
        float distanceSquared = (point - Center).LengthSquared();
        return distanceSquared <= (Radius * Radius);
    }
};

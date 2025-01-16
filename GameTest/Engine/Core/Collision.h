#pragma once
#include "Engine/Math/Vector3.h"
#include <vector>
struct Collision
{
    Collision(const unsigned int ID1, const unsigned int ID2, const FVector3 &contactPoint, const FVector3 &normal,
              const float penetration)
        : contactPoint(contactPoint), normal(normal), penetration(penetration), ID1(ID1), ID2(ID2)
    {
    }
    FVector3 contactPoint = {0, 0, 0};
    FVector3 normal = {0, 0, 0};
    float penetration = 0.0f;
    unsigned int ID1 = 0;
    unsigned int ID2 = 0;
};

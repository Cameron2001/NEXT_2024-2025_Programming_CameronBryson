#pragma once
#include "Engine/Math/Vector3.h"

struct Collision
{
    Collision(const unsigned int ID1, const unsigned int ID2, const float penetration, const FVector3 &normal)
        : ID1(ID1), ID2(ID2), penetration(penetration), normal(normal)
    {
    }
    // Should we store collision type?
    //  DynamicVSStatic, DynamicVSDynamic, StaticVSStatic
    FVector3 normal = {0, 0, 0};
    float penetration = 0.0f;
    unsigned int ID1 = 0;
    unsigned int ID2 = 0;
};

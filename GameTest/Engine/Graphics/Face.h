#pragma once
#include "Engine/Math/Vector3.h"

struct Face
{
    Face(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2, const FVector3 &normal)
        : v0(v0), v1(v1), v2(v2), normal(normal)
    {
    }
    Face(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2) : v0(v0), v1(v1), v2(v2)
    {
        normal = {0,0,0};
    }
    FVector3 v0;
    FVector3 v1;
    FVector3 v2;
    FVector3 normal;
};
#pragma once
#include "Engine/Math/Vector3.h"

struct Vertex
{
    Vertex(const FVector3 &position) : position(position)
    {
    }

    Vertex(float x, float y, float z) : position(FVector3(x, y, z))
    {
    }

    FVector3 position;
    //FVector3 normal;
    //Can be expanded later
};
#pragma once
#include "Engine/Math/Vector3.h"

struct Face
{
    Face(unsigned int index1, unsigned int index2, unsigned int index3) : index1(index1), index2(index2), index3(index3)
    {
    }

    unsigned int index1, index2, index3;
};
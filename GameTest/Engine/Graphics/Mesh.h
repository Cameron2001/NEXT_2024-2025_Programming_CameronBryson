#pragma once
#include <vector>
#include "Engine/Graphics/Triangle.h"
#include <Engine/Math/Matrix4.h>

struct Mesh
{
    Mesh(const std::vector<Triangle> &triangles) : triangles(triangles)
    {
    }
    std::vector<Triangle> triangles;
};

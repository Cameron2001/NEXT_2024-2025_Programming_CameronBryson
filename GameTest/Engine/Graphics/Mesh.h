#pragma once
#include <vector>
#include "Engine/Graphics/Triangle.h"

struct Mesh
{
    explicit Mesh(const std::vector<Triangle3D> &triangles) : triangles(triangles)
    {
    }
    std::vector<Triangle3D> triangles;
};

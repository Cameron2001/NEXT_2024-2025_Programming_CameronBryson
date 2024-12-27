#pragma once
#include <vector>
#include "Engine/Graphics/Triangle.h"
#include <Engine/Math/Matrix4.h>

struct Mesh
{
    explicit Mesh(const std::vector<Triangle3D> &triangles) : triangles(triangles)
    {
    }
    std::vector<Triangle3D> triangles;
};

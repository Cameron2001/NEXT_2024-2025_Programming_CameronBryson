#pragma once
#include <vector>
#include "Engine/Graphics/Face.h"
#include <Engine/Math/Matrix4.h>

struct Mesh
{
    Mesh(const std::vector<Face> &faces) : faces(faces)
    {
    }
    std::vector<Face> faces;
};
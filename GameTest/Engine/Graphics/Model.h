#pragma once
#include <vector>
#include "Engine/Graphics/Mesh.h"
#include <Engine/Math/Matrix4.h>

struct Model
{
    explicit Model(const std::vector<Mesh> &meshes) : meshes(meshes)
    {
    }

    std::vector<Mesh> meshes;
};
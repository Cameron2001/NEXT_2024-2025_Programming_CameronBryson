#pragma once
#include <vector>
#include "Game/Graphics/Mesh.h"
#include <Game/Math/Matrix4.h>

struct Model
{
    explicit Model(const std::vector<Mesh> &meshes) : meshes(meshes)
    {
    }

    std::vector<Mesh> meshes;
};
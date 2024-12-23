#pragma once
#include <vector>
#include "Engine/Graphics/Mesh.h"
#include <Engine/Math/Matrix4.h>

class Model
{
public:
    explicit Model(const std::vector<Mesh> &meshes) : meshes(meshes)
    {
    }

    std::vector<Mesh> meshes;
};
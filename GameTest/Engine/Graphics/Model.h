#pragma once
#include <vector>
#include "Engine/Graphics/Mesh.h"
struct Model
{
	Model(const std::vector<Mesh>& meshes) : meshes(meshes) {}
	std::vector<Mesh> meshes;
};
#pragma once
#include <vector>
#include "Engine/Graphics/Vertex.h"
struct Mesh
{
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : vertices(vertices), indices(indices) {}
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};


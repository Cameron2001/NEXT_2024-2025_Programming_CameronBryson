#pragma once
#include <vector>
#include "Engine/Graphics/Vertex.h"
struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};


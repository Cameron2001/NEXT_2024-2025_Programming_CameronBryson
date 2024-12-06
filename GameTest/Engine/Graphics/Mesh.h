#pragma once
#include <vector>
#include "Engine/Graphics/Vertex.h"
#include "Engine/Graphics/Face.h"
#include <Engine/Math/Matrix4.h>
struct Mesh
{
	Mesh(const std::vector<Vertex>& vertices, const std::vector<Face>& faces) : vertices(vertices), faces(faces) 
	{
	}
	std::vector<Vertex> vertices;
	std::vector<Face> faces;
};


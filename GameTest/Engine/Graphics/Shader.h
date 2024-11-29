#pragma once
#include "Engine/Graphics/Mesh.h"
#include "Engine/Math/Matrix4.h"
class Shader
{
public:
	//need to pass in MVP matrix
	void DrawMesh(const Matrix4& MVP, const Mesh& mesh);
};


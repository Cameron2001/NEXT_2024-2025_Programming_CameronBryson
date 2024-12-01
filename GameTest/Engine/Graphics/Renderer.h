#pragma once
#include "Engine/Math/Matrix4.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4.h"

class Renderer
{
public:
	static void RenderMesh(const Mesh& mesh, const Matrix4& MVP);
	static void RenderModel(const Model& model, const Matrix4& MVP);
private:
	static FVector2 ProjectPointToScreen(const FVector3& point, const Matrix4& MVP);
};
//Things we need to render something. MVP matrix, mesh, shader, camera


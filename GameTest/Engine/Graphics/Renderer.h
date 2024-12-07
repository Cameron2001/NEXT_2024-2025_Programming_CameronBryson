#pragma once
#include "Camera.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector3.h"

class Renderer
{
public:
    static void RenderMesh(const Mesh &mesh, const Matrix4 &MVP);
    static void RenderModel(const Model &model, const Matrix4 &MVP);

private:
    static bool IsOnScreen(const FVector3 &point);
};

//Things we need to render something. MVP matrix, mesh, shader, camera
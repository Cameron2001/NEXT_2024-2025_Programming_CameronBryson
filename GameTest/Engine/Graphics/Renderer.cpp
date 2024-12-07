#include "stdafx.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Math/Vector4.h>
#include <limits>
#include "Camera.h"

void Renderer::RenderMesh(const Mesh &mesh, const Matrix4 &MVP)
{
    for (const auto &face : mesh.faces)
    {
        FVector3 mvpVertex0 = MVP * mesh.vertices[face.index1].position;
        FVector3 mvpVertex1 = MVP * mesh.vertices[face.index2].position;
        FVector3 mvpVertex2 = MVP * mesh.vertices[face.index3].position;

        if (!IsOnScreen(mvpVertex0) || !IsOnScreen(mvpVertex1) || !IsOnScreen(mvpVertex2))
        {
            continue;
        }
        //Backface culling
        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) - (
                                (mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant > 0)
        {
            continue;
        }
        Renderer2D::DrawLine(FVector2{mvpVertex0.X, mvpVertex0.Y}, FVector2{mvpVertex1.X, mvpVertex1.Y},
                             FVector3(1, 1, 1));
        Renderer2D::DrawLine(FVector2{mvpVertex1.X, mvpVertex1.Y}, FVector2{mvpVertex2.X, mvpVertex2.Y},
                             FVector3(1, 1, 1));
        Renderer2D::DrawLine(FVector2{mvpVertex2.X, mvpVertex2.Y}, FVector2{mvpVertex0.X, mvpVertex0.Y},
                             FVector3(1, 1, 1));

    }
}

void Renderer::RenderModel(const Model &model, const Matrix4 &MVP)
{
    for (const auto &mesh : model.meshes)
    {
        RenderMesh(mesh, MVP);
    }
}

bool Renderer::IsOnScreen(const FVector3 &point)
{
    return (point.X >= -1.0f && point.X <= 1.0f) &&
           (point.Y >= -1.0f && point.Y <= 1.0f) &&
           (point.Z >= -1.0f && point.Z <= 1.0f);
}
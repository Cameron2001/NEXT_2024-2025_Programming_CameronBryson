#include "stdafx.h"
#include "Renderer.h"
#include "Renderer2D.h"
//We are using virtual res so we do not need to convert to NDC
void Renderer::RenderMesh(const Mesh& mesh, const Matrix4& MVP)
{
	for (int i = 0; i < mesh.indices.size(); i += 2)
	{
		int index = mesh.indices[i];
		int nextIndex = mesh.indices[i + 1];

		FVector2 screenPoint = ProjectPointToScreen(mesh.vertices[index].position, MVP);
		FVector2 nextScreenPoint = ProjectPointToScreen(mesh.vertices[nextIndex].position, MVP);

		Renderer2D::DrawLine(screenPoint, nextScreenPoint, FVector3(1.0f, 1.0f, 1.0f));
	}
}

void Renderer::RenderModel(const Model& model, const Matrix4& MVP)
{
	for (auto& mesh : model.meshes)
	{
		RenderMesh(mesh, MVP);
	}
}

FVector2 Renderer::ProjectPointToScreen(const FVector3& point, const Matrix4& MVP)
{
	FVector3 transformedPoint = MVP * point;
	return FVector2(transformedPoint.X, transformedPoint.Y);
}


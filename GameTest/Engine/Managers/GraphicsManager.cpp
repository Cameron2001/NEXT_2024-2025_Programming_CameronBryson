#include "stdafx.h"
#include "GraphicsManager.h"

void GraphicsManager::LoadModel(const std::string& modelName, const std::string& path)
{
}

void GraphicsManager::DrawModel(const std::string& modelName)
{
	Model & model = m_modelMap[modelName];
	for (Mesh&mesh : model.meshes)
	{
		m_shader.DrawMesh(mesh);
	}
}

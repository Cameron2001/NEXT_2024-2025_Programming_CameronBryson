#include "stdafx.h"
#include "GraphicsManager.h"

void GraphicsManager::LoadModel(const std::string& modelName, const std::string& path)
{
}

Model& GraphicsManager::GetModel(const std::string& modelName)
{
	return m_modelMap[modelName];
}


void GraphicsManager::Shutdown()
{
}

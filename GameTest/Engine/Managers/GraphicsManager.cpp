#include "stdafx.h"
#include "GraphicsManager.h"

void GraphicsManager::AddModel(const std::string &modelName, const Model &model)
{
    m_modelMap[modelName] = std::make_shared<Model>(model);
}

void GraphicsManager::LoadModel(const std::string &modelName, const std::string &path)
{
}

Model &GraphicsManager::GetModel(const std::string &modelName)
{
    return *m_modelMap[modelName];
}


void GraphicsManager::Shutdown()
{
}
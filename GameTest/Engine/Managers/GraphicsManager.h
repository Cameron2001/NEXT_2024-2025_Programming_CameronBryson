#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Engine/Graphics/Model.h"

class GraphicsManager
{
public:
    void AddModel(const std::string &modelName, const Model &model);
    void LoadModel(const std::string &modelName, const std::string &path);
    Model &GetModel(const std::string &modelName);
    void Shutdown();

private:
    std::unordered_map<std::string, std::shared_ptr<Model>> m_modelMap;
};
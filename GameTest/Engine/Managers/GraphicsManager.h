#pragma once
#include <unordered_map>
#include <string>
#include "Engine/Graphics/Model.h"
#include "Engine/Graphics/Shader.h"
class GraphicsManager
{
public:
	void LoadModel(const std::string& modelName, const std::string& path);
	void DrawModel(const std::string& modelName);
private:
	std::unordered_map<std::string, Model> m_modelMap;
	Shader m_shader;
};


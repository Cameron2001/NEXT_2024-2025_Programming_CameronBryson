#pragma once
#include <memory>
#include <vector>
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Engine/Graphics/Camera.h"
class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem() = default;
	void Init(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<Camera> camera, std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<ModelComponent>> modelComponentArray, std::shared_ptr<ComponentArray<TextComponent>> textComponentArray);
	void Render();
	void Shutdown();
private:
	std::weak_ptr<GraphicsManager> m_graphicsManager; //Read
	std::weak_ptr<Camera> m_camera; //Read
	std::weak_ptr<ComponentArray<TransformComponent>> m_transformComponentArray; //Read
	std::weak_ptr<ComponentArray<ModelComponent>> m_modelComponentArray; //Read
	std::weak_ptr<ComponentArray<TextComponent>> m_textComponentArray; //Read
};


//Read:
//GraphicsManager
//Camera
//TransformComponent
//ModelComponent
//TextComponent

//Write:
//None

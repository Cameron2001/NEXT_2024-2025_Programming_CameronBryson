#pragma once
#include <memory>
#include <vector>
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem() = default;
	void Init();
	void Render();
	void Shutdown();
private:
	std::shared_ptr<GraphicsManager> m_graphicsManager;
	std::shared_ptr<ComponentArray<TransformComponent>> m_transformComponents;
	std::shared_ptr<ComponentArray<ModelComponent>> m_modelComponents;
	std::shared_ptr<ComponentArray<TextComponent>> m_textComponents;
	//Needs to get passed these pointers on creation
	// 
	//What this system needs:
	//graphics manager to grab models and shader
	//transform components
	//model components
	//text components
	//Camera to do rendering
	//create mvp matrix
	//send mesh and mvp matrix to shader

	//What we are going to do:
	//Get IDs of entities that have transform and model
	// Get Model from graphics manager and shader
	// Render model using transform
	//Get IDs of entties that have transform and text
	//Render text using transform

	

};


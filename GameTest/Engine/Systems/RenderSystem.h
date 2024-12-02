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

	//Optional:
	//Collider components if I want to draw colliders

	//What we are going to do:
	//Get IDs of entities that have transform and model
	// Get Model from graphics manager and shader
	// Render model using transform
	//Get IDs of entties that have transform and text
	//Render text using transform



	

};


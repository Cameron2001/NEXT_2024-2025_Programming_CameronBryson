#pragma once
#include <memory>
#include <vector>
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
class RenderView
{
public:
	RenderView();
	~RenderView() = default;
	void Init();
	void Render();
	void Shutdown();
private:
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


#pragma once
#include "Engine/Storage/Registry.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/GraphicsManager.h"

#include "Engine/Systems/RenderSystem.h"
class Scene
{
public:
	Scene();
	virtual ~Scene() = default;
	virtual void Init();
	virtual void LateInit();
	virtual void Update(const float dt);
	virtual void LateUpdate(const float dt);
	virtual void Render();
	virtual void LateRender();
	virtual void Shutdown();
	virtual void LateShutdown();
private:
	void InitComponentArrays();
protected:
	//Thinking about making them shared pointers instead for flexibility

	//Registry
	Registry m_registry;

	//Managers
	AudioManager m_audioManager;
	GraphicsManager m_graphicsManager;


	//Systems
	RenderSystem m_renderSystem;


};


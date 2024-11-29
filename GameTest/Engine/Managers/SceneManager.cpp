#include "stdafx.h"
#include "SceneManager.h"
#include "Engine/Core/Scene.h"

std::shared_ptr<Scene> SceneManager::m_currentScene = nullptr;
void SceneManager::Init()
{
	GetCurrentScene()->Init();
}

void SceneManager::LateInit()
{
	GetCurrentScene()->LateInit();
}

void SceneManager::Update(const float dt)
{
	GetCurrentScene()->Update(dt);
}

void SceneManager::LateUpdate(const float dt)
{
	GetCurrentScene()->LateUpdate(dt);
}

void SceneManager::Render()
{
	GetCurrentScene()->Render();
}

void SceneManager::LateRender()
{
	GetCurrentScene()->LateRender();
}

void SceneManager::Shutdown()
{
	GetCurrentScene()->Shutdown();
}

void SceneManager::LateShutdown()
{
	GetCurrentScene()->LateShutdown();
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

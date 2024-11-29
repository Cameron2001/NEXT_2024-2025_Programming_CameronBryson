#include "stdafx.h"
#include "SceneManager.h"
#include "Engine/Core/Scene.h"

std::shared_ptr<Scene> SceneManager::m_currentScene = nullptr;

std::shared_ptr<Scene> SceneManager::GetCurrentScene()
{
	return m_currentScene;
}

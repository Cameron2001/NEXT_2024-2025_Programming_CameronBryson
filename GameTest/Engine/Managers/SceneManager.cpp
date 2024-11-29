#include "stdafx.h"
#include "SceneManager.h"
#include "Engine/Core/Scene.h"

std::unique_ptr<Scene> SceneManager::m_currentScene = nullptr;

Scene& SceneManager::GetCurrentScene()
{
	return *m_currentScene;
}

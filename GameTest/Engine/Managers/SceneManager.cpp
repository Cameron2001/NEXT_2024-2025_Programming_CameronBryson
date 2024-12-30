#include "stdafx.h"
#include "SceneManager.h"
#include "Engine/Core/Scene.h"

std::shared_ptr<Scene> SceneManager::m_currentScene = nullptr;

Scene &SceneManager::GetCurrentScene()
{
    if (!m_currentScene)
    {
        throw std::runtime_error("No current scene loaded.");
    }
    return *m_currentScene;
}

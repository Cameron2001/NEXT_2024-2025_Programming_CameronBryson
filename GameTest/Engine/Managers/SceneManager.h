// SceneManager.h
#pragma once
#include <memory>
#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/EventManager.h"
#include "Engine/Core/Scene.h"
class SceneManager
{
  public:
    SceneManager()
        : m_audioManager(std::make_shared<AudioManager>()), m_graphicsManager(std::make_shared<GraphicsManager>()),
          m_eventManager(std::make_shared<EventManager>())
    {
    }
    template <typename T> void LoadScene()
    {
        if (m_currentScene)
        {
            m_currentScene->Shutdown();
        }
        m_currentScene = std::make_shared<T>(m_audioManager, m_graphicsManager,m_eventManager);
        m_currentScene->Init();
        m_currentScene->LateInit();
    }

    std::shared_ptr<Scene> GetCurrentScene() const
    {
        return m_currentScene;
    }

  private:
    std::shared_ptr<Scene> m_currentScene;
    std::shared_ptr<AudioManager> m_audioManager;
    std::shared_ptr<GraphicsManager> m_graphicsManager;
    std::shared_ptr<EventManager> m_eventManager;
};

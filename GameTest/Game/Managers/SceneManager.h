// SceneManager.h
#pragma once
#include <memory>
#include "Game/Managers/GraphicsManager.h"
#include "Game/Managers/EventManager.h"
#include "Game/Managers/PlayerManager.h"
#include "Game/Core/Scene.h"
#include <string>
#include "Game/Scenes/MainMenuScene.h"
#include "Game/Scenes/LevelOneScene.h"
#include "Game/Scenes/ResultsScene.h"
class SceneManager : public std::enable_shared_from_this<SceneManager>
{
  public:
    SceneManager()
        : m_graphicsManager(std::make_shared<GraphicsManager>()), m_eventManager(std::make_shared<EventManager>()),
          m_scoreManager(std::make_shared<PlayerManager>())
    {

    }
    void BindSceneChangeEvent()
    {
        m_eventManager->AddListener<const char*>("SceneChange", shared_from_this(), &SceneManager::OnSceneChangeEvent);
    }
    template <typename T> void LoadScene()
    {
        if (m_currentScene)
        {
            m_currentScene->Shutdown();
        }
        m_currentScene = std::make_shared<T>(m_graphicsManager, m_eventManager, m_scoreManager);
        m_currentScene->Init();
        m_currentScene->LateInit();
    }

    std::shared_ptr<Scene> GetCurrentScene() const
    {
        return m_currentScene;
    }
    void OnSceneChangeEvent(const char* name)
    {

        if(name == "MainMenu")
        {
            LoadScene<MainMenuScene>();
        }
        else if (name == "LevelOne")
        {
            LoadScene<LevelOneScene>();
        }
        else if (name == "Results")
        {
            LoadScene<ResultsScene>();
        }
    }

  private:

    std::shared_ptr<Scene> m_currentScene;
    std::shared_ptr<GraphicsManager> m_graphicsManager;
    std::shared_ptr<EventManager> m_eventManager;
    std::shared_ptr<PlayerManager> m_scoreManager;
};

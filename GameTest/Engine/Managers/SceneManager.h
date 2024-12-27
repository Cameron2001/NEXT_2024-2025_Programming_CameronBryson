#pragma once
#include <memory>
class Scene;

class SceneManager
{
  public:
    template <typename T> static void LoadScene()
    {
        if (m_currentScene)
        {
            m_currentScene->Shutdown();
        }
        m_currentScene = std::make_shared<T>();
        m_currentScene->Init();
        m_currentScene->LateInit();
    }

    static Scene &GetCurrentScene();

  private:
    static std::shared_ptr<Scene> m_currentScene;
};

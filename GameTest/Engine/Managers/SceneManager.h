#pragma once
#include <memory>
class Scene;

class SceneManager
{
public:
    template <typename T>
    static void LoadScene()
    {
        if (m_currentScene)
        {
            m_currentScene->Shutdown();
        }
        m_currentScene = std::make_unique<T>();
        m_currentScene->Init();
    }

    static Scene &GetCurrentScene();

private:
    static std::unique_ptr<Scene> m_currentScene;


};
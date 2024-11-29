#pragma once
#include <memory>
class Scene;
class SceneManager
{
public:
	static void Init();
	static void LateInit();
	static void Update(const float dt);
	static void LateUpdate(const float dt);
	static void Render();
	static void LateRender();
	static void Shutdown();
	static void LateShutdown();

	template <typename T>
	static void LoadScene()
	{
		if (m_currentScene)
		{
			m_currentScene->Shutdown();
		}
		m_currentScene = std::make_shared<T>();
		m_currentScene->Init();
	}

	static std::shared_ptr<Scene> GetCurrentScene();
private:
	static std::shared_ptr<Scene> m_currentScene;


};


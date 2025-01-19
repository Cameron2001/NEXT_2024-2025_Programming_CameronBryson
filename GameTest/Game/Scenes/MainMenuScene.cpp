#include "stdafx.h"
#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(std::shared_ptr<GraphicsManager> graphicsManager,
                             std::shared_ptr<EventManager> eventManager, std::shared_ptr<PlayerManager> scoreManager)
    : Scene(graphicsManager, eventManager, scoreManager),
      m_menuSystem(std::make_shared<MenuSystem>(eventManager.get()))
{
}

void MainMenuScene::Init()
{
    Scene::Init();

}

void MainMenuScene::LateInit()
{
    Scene::LateInit();
}

void MainMenuScene::Update(float dt)
{
    Scene::Update(dt);

    m_menuSystem->Update(dt);
}

void MainMenuScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
}

void MainMenuScene::Render()
{
    Scene::Render();
}

void MainMenuScene::LateRender()
{
    Scene::LateRender();
}

void MainMenuScene::Shutdown()
{
    Scene::Shutdown();
}

void MainMenuScene::LateShutdown()
{
    Scene::LateShutdown();
}

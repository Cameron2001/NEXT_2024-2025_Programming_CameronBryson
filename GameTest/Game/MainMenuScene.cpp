#include "stdafx.h"
#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(std::shared_ptr<AudioManager> audioManager,
                             std::shared_ptr<GraphicsManager> graphicsManager,
                             std::shared_ptr<EventManager> eventManager)
    : Scene(audioManager,graphicsManager,eventManager)
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

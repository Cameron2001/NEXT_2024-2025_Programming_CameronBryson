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
    const auto mainMenuText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(mainMenuText, "Main Menu", FVector2(500.0f, 500.0f));

    const auto levelOneText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(levelOneText, "Press 1 for Level One", FVector2(500.0f, 400.0f));

    const auto controlsText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(controlsText, "WASD to move the arrow. Hold and release space bar to shoot!", FVector2(500.0f, 300.0f));


    const auto escapeText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(escapeText, "Press ESC to Exit", FVector2(500.0f, 200.0f));




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

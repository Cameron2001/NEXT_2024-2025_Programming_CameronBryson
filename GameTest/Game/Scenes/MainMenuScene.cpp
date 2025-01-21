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
    auto mainMenuText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(mainMenuText, "Main Menu", FVector2(500.0f, 500.0f));

    auto levelOneText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(levelOneText, "Press 1 for Level One", FVector2(500.0f, 400.0f));

    auto levelTwoText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(levelTwoText, "Press 2 for Level Two", FVector2(500.0f, 300.0f));


    auto escapeText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(escapeText, "Press ESC to Exit", FVector2(500.0f, 100.0f));

    auto model = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(model, FVector3(-5.0f, 5.0f, 5.0f), FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ModelComponent>(model, "SphereOBJ");




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

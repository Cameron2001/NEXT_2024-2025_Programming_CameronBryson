#include "stdafx.h"
#include "LevelTwoScene.h"

LevelTwoScene::LevelTwoScene(std::shared_ptr<GraphicsManager> graphicsManager,
                             std::shared_ptr<EventManager> eventManager, std::shared_ptr<PlayerManager> scoreManager)
    : PlayScene(graphicsManager, eventManager, scoreManager)
{
}

void LevelTwoScene::Init()
{
    PlayScene::Init();
}

void LevelTwoScene::LateInit()
{
    PlayScene::LateInit();
}

void LevelTwoScene::Update(float dt)
{
    PlayScene::Update(dt);
}

void LevelTwoScene::LateUpdate(float dt)
{
    PlayScene::LateUpdate(dt);
}

void LevelTwoScene::Render()
{
    PlayScene::Render();
}

void LevelTwoScene::LateRender()
{
    PlayScene::LateRender();
}

void LevelTwoScene::Shutdown()
{
    PlayScene::Shutdown();
}

void LevelTwoScene::LateShutdown()
{
    PlayScene::LateShutdown();
}

void LevelTwoScene::BuildLevelTwo()
{
}

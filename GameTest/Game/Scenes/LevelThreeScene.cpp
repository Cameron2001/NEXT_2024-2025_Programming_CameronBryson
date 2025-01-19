#include "stdafx.h"
#include "LevelThreeScene.h"

LevelThreeScene::LevelThreeScene(std::shared_ptr<GraphicsManager> graphicsManager,
                                 std::shared_ptr<EventManager> eventManager,
                                 std::shared_ptr<PlayerManager> scoreManager)
    : PlayScene(graphicsManager, eventManager, scoreManager)
{
}

void LevelThreeScene::Init()
{
    PlayScene::Init();
}

void LevelThreeScene::LateInit()
{
    PlayScene::LateInit();
}

void LevelThreeScene::Update(float dt)
{
    PlayScene::Update(dt);
}

void LevelThreeScene::LateUpdate(float dt)
{
    PlayScene::LateUpdate(dt);
}

void LevelThreeScene::Render()
{
    PlayScene::Render();
}

void LevelThreeScene::LateRender()
{
    PlayScene::LateRender();
}

void LevelThreeScene::Shutdown()
{
    PlayScene::Shutdown();
}

void LevelThreeScene::LateShutdown()
{
    PlayScene::LateShutdown();
}

void LevelThreeScene::BuildLevelThree()
{

}

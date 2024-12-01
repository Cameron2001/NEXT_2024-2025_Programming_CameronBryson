#include "stdafx.h"
#include "PlayScene.h"
#include "Game/GameComponents.h"

PlayScene::PlayScene() : Scene()
{
}

void PlayScene::Init()
{
	Scene::Init();
	auto player = m_registry->CreateEntity();
	m_registry->AddComponent<TransformComponent>(player, FVector3(0.0f, 0.0f, -10.0f), FVector3(45.0f, 45.0f, 45.0f), FVector3(0.1f, 0.1f, 0.1f));
	m_registry->AddComponent<PlayerComponent>(player);
	m_registry->AddComponent<ModelComponent>(player, "Cube");
}

void PlayScene::LateInit()
{
	Scene::LateInit();
}

void PlayScene::Update(const float dt)
{
	Scene::Update(dt);
}

void PlayScene::LateUpdate(const float dt)
{
	Scene::LateUpdate(dt);
}

void PlayScene::Render()
{
	Scene::Render();
}

void PlayScene::LateRender()
{
	Scene::LateRender();
}

void PlayScene::Shutdown()
{
	Scene::Shutdown();
}

void PlayScene::LateShutdown()
{
	Scene::LateShutdown();
}

void PlayScene::InitComponentArrays()
{
	m_registry->CreateComponentArray<PlayerComponent>();

}

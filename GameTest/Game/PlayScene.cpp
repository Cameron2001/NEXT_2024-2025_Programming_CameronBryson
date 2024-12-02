#include "stdafx.h"
#include "PlayScene.h"
#include "Game/GameComponents.h"
#include "Engine/Core/Event.h"
Event<int> m_event;
void test(int test)
{

}
PlayScene::PlayScene() : Scene()
{
}

void PlayScene::Init()
{
	Scene::Init();
	m_event.AddListener([](int value) { test(value); });
	auto player = m_registry->CreateEntity();
	m_registry->AddComponent<TransformComponent>(player, FVector3(0.0f, 0.0f, -10.0f), FVector3(45.0f, 45.0f, 45.0f), FVector3(0.1f, 0.1f, 0.1f));
	m_registry->AddComponent<PlayerComponent>(player);
	m_registry->AddComponent<ModelComponent>(player, "Cube");
	m_event.Notify(10);


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

#include "stdafx.h"
#include "Scene.h"
#include "Engine/Core/Components.h"

Scene::Scene()
{
}

void Scene::Init()
{
	InitComponentArrays();
}

void Scene::LateInit()
{
}

void Scene::Update(const float dt)
{
}

void Scene::LateUpdate(const float dt)
{
}

void Scene::Render()
{
}

void Scene::LateRender()
{
}

void Scene::Shutdown()
{
}

void Scene::LateShutdown()
{
}

void Scene::InitComponentArrays()
{
	m_registry.CreateComponentArray<TransformComponent>();
	m_registry.CreateComponentArray<ColliderComponent>();
	m_registry.CreateComponentArray<BoxBoundsComponent>();
	m_registry.CreateComponentArray<SphereBoundsComponent>();
	m_registry.CreateComponentArray<RigidBodyComponent>();
	m_registry.CreateComponentArray<TextComponent>();
}

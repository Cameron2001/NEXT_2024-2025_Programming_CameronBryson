#include "stdafx.h"
#include "Scene.h"
#include "Engine/Core/Components.h"

Scene::Scene() 
{
	m_registry = std::make_shared<Registry>();
	m_audioManager = std::make_shared<AudioManager>();
	m_graphicsManager = std::make_shared<GraphicsManager>();
	m_collisionManager = std::make_shared<CollisionManager>();
	m_camera = std::make_shared<Camera>(FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 45.0f);
}

void Scene::Init()
{
	InitComponentArrays();
	m_renderView.Init(m_graphicsManager,m_camera, m_registry->GetComponentArray<TransformComponent>(), m_registry->GetComponentArray<ModelComponent>(), m_registry->GetComponentArray<TextComponent>());
	m_collisionView.Init(m_collisionManager,m_registry->GetComponentArray<TransformComponent>(), m_registry->GetComponentArray<ColliderComponent>(), m_registry->GetComponentArray<RigidBodyComponent>(), m_registry->GetComponentArray<BoxBoundsComponent>(), m_registry->GetComponentArray<SphereBoundsComponent>());
	m_physicsController.Init(m_registry->GetComponentArray<TransformComponent>(), m_registry->GetComponentArray<RigidBodyComponent>());
	m_collisionController.Init(m_collisionManager, m_registry->GetComponentArray<TransformComponent>(), m_registry->GetComponentArray<RigidBodyComponent>());
}

void Scene::LateInit()
{
}

void Scene::Update(const float dt)
{
	m_physicsController.Update(dt);
	m_collisionView.Update(dt);
	m_collisionController.Update(dt);
}

void Scene::LateUpdate(const float dt)
{
}

void Scene::Render()
{
	m_renderView.Render();
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
	m_registry->CreateComponentArray<TransformComponent>();
	m_registry->CreateComponentArray<ColliderComponent>();
	m_registry->CreateComponentArray<BoxBoundsComponent>();
	m_registry->CreateComponentArray<SphereBoundsComponent>();
	m_registry->CreateComponentArray<RigidBodyComponent>();
	m_registry->CreateComponentArray<TextComponent>();
}

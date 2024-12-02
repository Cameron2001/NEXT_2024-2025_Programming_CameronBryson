#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}


void PhysicsSystem::Init(std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray)
{
	m_transformComponents = transformComponentArray;
	m_rigidBodyComponents = rigidbodyComponentArray;
}

void PhysicsSystem::Update(const float dt)
{
	auto transformComponents = m_transformComponents.lock();
	auto rigidBodyComponents = m_rigidBodyComponents.lock();
	if (!transformComponents || !rigidBodyComponents)
	{
		return;
	}
}

void PhysicsSystem::Shutdown()
{
}

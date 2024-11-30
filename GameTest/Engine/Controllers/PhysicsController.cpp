#include "stdafx.h"
#include "PhysicsController.h"

PhysicsController::PhysicsController()
{
}


void PhysicsController::Init(std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray)
{
	m_transformComponents = transformComponentArray;
	m_rigidBodyComponents = rigidbodyComponentArray;
}

void PhysicsController::Update(const float dt)
{
	auto transformComponents = m_transformComponents.lock();
	auto rigidBodyComponents = m_rigidBodyComponents.lock();
	if (!transformComponents || !rigidBodyComponents)
	{
		return;
	}
}

void PhysicsController::Shutdown()
{
}

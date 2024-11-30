#include "stdafx.h"
#include "CollisionController.h"

CollisionController::CollisionController()
{
}

void CollisionController::Init(std::shared_ptr<CollisionManager> collisionManager, std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidboyComponentArray)
{
	m_collisionManager = collisionManager;
	m_transformComponentArray = transformComponentArray;
	m_rigidBodyComponentArray = rigidboyComponentArray;

}

void CollisionController::Update(float dt)
{
	auto collisionManager = m_collisionManager.lock();
	auto transformComponentArray = m_transformComponentArray.lock();
	auto rigidboyComponentArray = m_rigidBodyComponentArray.lock();
	if (!collisionManager || !transformComponentArray || !rigidboyComponentArray)
	{
		return;
	}
}

void CollisionController::Shutdown()
{
}

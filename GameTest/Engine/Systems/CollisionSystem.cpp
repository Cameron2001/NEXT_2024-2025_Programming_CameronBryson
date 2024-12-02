#include "stdafx.h"
#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
{
}


void CollisionSystem::Init(std::shared_ptr<CollisionManager> collisionManager, std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<ColliderComponent>> colliderComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidBodyComponentArray, std::shared_ptr<ComponentArray<BoxBoundsComponent>> boxBoundsComponentArray, std::shared_ptr<ComponentArray<SphereBoundsComponent>> sphereBoundsComponentArray)
{
	m_collisionManager = collisionManager;
	m_transformComponentArray = transformComponentArray;
	m_colliderComponentArray = colliderComponentArray;
	m_rigidBodyComponentArray = rigidBodyComponentArray;
	m_boxBoundsComponentArray = boxBoundsComponentArray;
	m_sphereBoundsComponentArray = sphereBoundsComponentArray;
}

void CollisionSystem::Update(const float dt)
{
	auto collisionManager = m_collisionManager.lock();
	auto transformComponentArray = m_transformComponentArray.lock();
	auto colliderComponentArray = m_colliderComponentArray.lock();
	auto rigidBodyComponentArray = m_rigidBodyComponentArray.lock();
	auto boxBoundsComponentArray = m_boxBoundsComponentArray.lock();
	auto sphereBoundsComponentArray = m_sphereBoundsComponentArray.lock();
	if (!collisionManager || !transformComponentArray || !colliderComponentArray || !rigidBodyComponentArray || !boxBoundsComponentArray || !sphereBoundsComponentArray)
	{
		return;
	}
}

void CollisionSystem::Shutdown()
{
}

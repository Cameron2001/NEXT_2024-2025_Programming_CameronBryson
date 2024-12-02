#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Engine/Managers/CollisionManager.h"
class CollisionSystem
{
public:
	CollisionSystem();
	~CollisionSystem() = default;
	void Init(std::shared_ptr<CollisionManager> collisionManager,
		std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray,
		std::shared_ptr<ComponentArray<ColliderComponent>> colliderComponentArray,
		std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidBodyComponentArray,
		std::shared_ptr<ComponentArray<BoxBoundsComponent>> boxBoundsComponentArray,
		std::shared_ptr<ComponentArray<SphereBoundsComponent>> sphereBoundsComponentArray);
	void Update(const float dt);
	void Shutdown();
private:
	std::weak_ptr<CollisionManager> m_collisionManager; //Write
	std::weak_ptr<ComponentArray<TransformComponent>> m_transformComponentArray; //Read
	std::weak_ptr<ComponentArray<ColliderComponent>> m_colliderComponentArray; //Read
	std::weak_ptr<ComponentArray<RigidBodyComponent>> m_rigidBodyComponentArray; //Read
	std::weak_ptr<ComponentArray<BoxBoundsComponent>> m_boxBoundsComponentArray; //Read
	std::weak_ptr<ComponentArray<SphereBoundsComponent>> m_sphereBoundsComponentArray; //Read

	//Things we need
	//Transform components
	//Collider components
	//RigidBody components
	//Box collider
	//Sphere collider
};
//What we need
//Transform components
//Collider components
//RigidBody components
//Box collider
//Sphere collider


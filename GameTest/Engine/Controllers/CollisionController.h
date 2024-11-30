#pragma once
#include <memory>
#include "Engine/Managers/CollisionManager.h"
#include "Engine/Core/Components.h"
#include "Engine/Storage/ComponentArray.h"
class CollisionController
{
public:
	CollisionController();
	~CollisionController() = default;
	void Init(std::shared_ptr<CollisionManager> collisionManager, std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray,std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidboyComponentArray);
	void Update(float dt);
	void Shutdown();
private:
	std::weak_ptr<CollisionManager> m_collisionManager;
	std::weak_ptr<ComponentArray<TransformComponent>> m_transformComponentArray;
	std::weak_ptr<ComponentArray<RigidBodyComponent>> m_rigidBodyComponentArray;
};


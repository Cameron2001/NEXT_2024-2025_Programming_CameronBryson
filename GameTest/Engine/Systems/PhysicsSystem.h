#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem() = default;
	void Init(std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray, std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray);
	void Update(const float dt);
	void Shutdown();
private:
	std::weak_ptr<ComponentArray<TransformComponent>> m_transformComponents; //Write
	std::weak_ptr<ComponentArray<RigidBodyComponent>> m_rigidBodyComponents; //Read and write
	//Things we need:
	//Transform components
	//RigidBody components
};


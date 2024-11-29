#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
class PhysicsController
{
public:
	PhysicsController();
	~PhysicsController() = default;
	void Init();
	void Update(const float dt);
	void Shutdown();
private:
	//Things we need:
	//Transform components
	//RigidBody components
};


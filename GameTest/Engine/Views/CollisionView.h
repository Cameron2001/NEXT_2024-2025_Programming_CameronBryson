#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
class CollisionView
{
public:
	CollisionView();
	~CollisionView() = default;
	void Init();
	void Update(const float dt);
	void Shutdown();
private:
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


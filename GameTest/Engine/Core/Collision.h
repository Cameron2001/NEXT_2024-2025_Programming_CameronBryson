#pragma once
#include "Engine/Math/Vector3.h"
#include "Engine/Core/Components.h"
class Collision
{
public:
private:
	//Think about what to do if components have been destroyed
	//Should I store components or the IDs and then grab the components
	//Do i want this class to be a struct and just data. Or do i want my resolve function in here
	float m_penetration = 0.0f;
	FVector3 m_normal = { 0,0,0 };
	RigidBodyComponent* m_rigidBodyA = nullptr;
	RigidBodyComponent* m_rigidBodyB = nullptr;
	TransformComponent* m_transformA = nullptr;
	TransformComponent* m_transformB = nullptr;

};


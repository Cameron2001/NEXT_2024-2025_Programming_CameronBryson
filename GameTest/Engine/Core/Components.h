#pragma once
#include "Engine/Math/Vector3.h"
#include <string>
struct TransformComponent
{
	TransformComponent()
		: Position(FVector3(0.0f, 0.0f, 0.0f)), Rotation(FVector3(0.0f, 0.0f, 0.0f)), Scale(FVector3(1.0f, 1.0f, 1.0f))
	{
	}
	TransformComponent(const FVector3& position, const FVector3& rotation, const FVector3& scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	}
	FVector3 Position;
	FVector3 Rotation;
	FVector3 Scale;
};
struct ColliderComponent
{

};
struct BoxBoundsComponent
{
	BoxBoundsComponent() : extents(FVector3(0.0f, 0.0f, 0.0f))
	{
	}
	BoxBoundsComponent(const FVector3& extents)
		: extents(extents)
	{
	}
	FVector3 extents;
};
struct SphereBoundsComponent
{
	SphereBoundsComponent() : radius(0.0f)
	{
	}
	SphereBoundsComponent(float radius)
		: radius(radius)
	{
	}
	float radius;
};
struct RigidBodyComponent
{

	float linearDrag;
	FVector3 linearVelocity;
	FVector3 linearAcceleration;
	float angularDrag;
	FVector3 angularVelocity;
	FVector3 angularAcceleration;
};
struct TextComponent
{
	std::string text;
};
struct ModelComponent
{
	std::string modelName;
};

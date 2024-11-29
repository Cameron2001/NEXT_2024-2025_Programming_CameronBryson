#pragma once
#include "Engine/Math/Vector3.h"
#include <string>
struct CTransform
{
	CTransform()
		: Position(FVector3(0.0f, 0.0f, 0.0f)), Rotation(FVector3(0.0f, 0.0f, 0.0f)), Scale(FVector3(1.0f, 1.0f, 1.0f))
	{
	}
	CTransform(const FVector3& position, const FVector3& rotation, const FVector3& scale)
		: Position(position), Rotation(rotation), Scale(scale)
	{
	}
	FVector3 Position;
	FVector3 Rotation;
	FVector3 Scale;
};
struct CCollider
{

};
struct CBoxBounds
{
	CBoxBounds() : extents(FVector3(0.0f, 0.0f, 0.0f))
	{
	}
	CBoxBounds(const FVector3& extents)
		: extents(extents)
	{
	}
	FVector3 extents;
};
struct CSphereBounds
{
	CSphereBounds() : radius(0.0f)
	{
	}
	CSphereBounds(float radius)
		: radius(radius)
	{
	}
	float radius;
};
struct CRigidBody
{

	float linearDrag;
	FVector3 linearVelocity;
	FVector3 linearAcceleration;
	float angularDrag;
	FVector3 angularVelocity;
	FVector3 angularAcceleration;
};
struct CText
{
	std::string text;
};
struct CModel
{
	std::string modelName;
};

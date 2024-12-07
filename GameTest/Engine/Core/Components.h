#pragma once
#include "Engine/Math/Vector3.h"
#include <string>

struct TransformComponent
{
    TransformComponent()
        : Position(FVector3(0.0f, 0.0f, 0.0f)), Rotation(FVector3(0.0f, 0.0f, 0.0f)), Scale(FVector3(1.0f, 1.0f, 1.0f))
    {
    }

    TransformComponent(const FVector3 &position, const FVector3 &rotation, const FVector3 &scale)
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

    BoxBoundsComponent(const FVector3 &extents)
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
    RigidBodyComponent() = default;
    float linearDrag = 0;
    FVector3 linearVelocity = {0, 0, 0};
    FVector3 linearAcceleration = {0, 0, 0};
    float angularDrag = 0;
    FVector3 angularVelocity = {0, 0, 0};
    FVector3 angularAcceleration = {0, 0, 0};
};

struct TextComponent
{
    std::string text;
};

struct ModelComponent
{
    ModelComponent(const std::string &modelName)
        : modelName(modelName)
    {
    }

    std::string modelName;
};
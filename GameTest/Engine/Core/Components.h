#pragma once
#include "Engine/Math/Vector3.h"
#include <string>
#include <Engine/Math/Vector2.h>
constexpr unsigned int LAYER_DEFAULT = 1 << 0;
enum class ColliderType : unsigned int
{
    Box,
    Sphere
};
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
struct BoxBoundsComponent
{
    BoxBoundsComponent() : extents(FVector3(0.0f, 0.0f, 0.0f))
    {
    }

    explicit BoxBoundsComponent(const FVector3 &extents) : extents(extents)
    {
    }

    FVector3 extents;
};

struct SphereBoundsComponent
{
    SphereBoundsComponent() : radius(0.0f)
    {
    }

    explicit SphereBoundsComponent(const float radius) : radius(radius)
    {
    }

    float radius;
};

struct RigidBodyComponent
{
    RigidBodyComponent() = default;
    RigidBodyComponent(const float linearDrag, const float angularDrag)
        : linearDrag(linearDrag), angularDrag(angularDrag)
    {
    }
    RigidBodyComponent(const float linearDrag, const float angularDrag, const FVector3 &initialLinearAcceleration,
                       const FVector3 &initialAngularAcceleration)
        : linearDrag(linearDrag), linearAcceleration(initialLinearAcceleration), angularDrag(angularDrag),
          angularAcceleration(initialAngularAcceleration)
    {
    }
    FVector3 linearVelocity = {0, 0, 0};
    FVector3 linearAcceleration = {0, 0, 0};
    FVector3 angularVelocity = {0, 0, 0};
    FVector3 angularAcceleration = {0, 0, 0};
    float linearDrag = 0;
    float angularDrag = 0;
    float mass = 1.0f;
};

struct TextComponent
{
    std::string text;
};

struct ModelComponent
{
    explicit ModelComponent(std::string modelName) : modelName(std::move(modelName))
    {
    }

    std::string modelName;
};
struct ParticleComponent
{
    ParticleComponent(const FVector2 &position, const float rotation, const FVector2 &linearVelocity,
                      const float angularVelocity, const float lifetime)
        : position(position), rotation(rotation), linearVelocity(linearVelocity), angularVelocity(angularVelocity),
          lifetime(lifetime)
    {
    }
    FVector2 position = {0, 0};
    FVector2 linearVelocity = {0, 0};
    float rotation = 0;
    float angularVelocity = 0;
    float lifetime = 0;
    float age = 0;
};
struct ColliderComponent
{
    ColliderComponent(const ColliderType type, bool isDynamic)
        : type(type), layer(LAYER_DEFAULT), mask(0xFFFFFFFF), isDynamic(isDynamic), isTrigger(false)
    {
    }
    ColliderType type;
    unsigned int layer;
    unsigned int mask;
    bool isDynamic;
    bool isTrigger; // Used during collision resolution
};

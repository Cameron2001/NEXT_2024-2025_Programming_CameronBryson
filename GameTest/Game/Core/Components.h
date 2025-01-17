#pragma once
#include "Game/Math/Vector3.h"
#include <Game/Math/Quaternion.h>
#include <Game/Math/Vector2.h>
#include <Game/Math/Matrix3.h>
#include <string>
constexpr unsigned int LAYER_DEFAULT = 1 << 0;
enum class ColliderType : unsigned int
{
    Box,
    Sphere
};
struct TransformComponent
{
    TransformComponent()
        : Rotation(Quaternion()), Position(FVector3(0.0f, 0.0f, 0.0f)), Scale(FVector3(1.0f, 1.0f, 1.0f))
    {
    }

    TransformComponent(const FVector3 &position, const FVector3 &rotation, const FVector3 &scale)
        : Rotation(Quaternion(rotation)), Position(position), Scale(scale)
    {
    }

    Quaternion Rotation;
    FVector3 Position;
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
        : force(initialLinearAcceleration), torque(initialAngularAcceleration), linearDrag(linearDrag),
          angularDrag(angularDrag)
    {
    }
    Matrix3 localInverseInertiaTensor;
    FVector3 linearVelocity = {0, 0, 0};
    FVector3 angularVelocity = {0, 0, 0};
    FVector3 force = {0, 0, 0};
    FVector3 torque = {0, 0, 0};
    float linearDrag = 0;
    float angularDrag = 0;
    float inverseMass = 1.0f;
    float elasticity = 1.0f;
    bool isInitialized = false;
};

struct TextComponent
{
    std::string text;
    FVector2 position;
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
        : position(position), linearVelocity(linearVelocity), rotation(rotation), angularVelocity(angularVelocity),
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
    bool isTrigger;
};
struct CameraFollowComponent
{
    CameraFollowComponent(const FVector3 &offset, float smoothSpeed) : offset(offset), smoothSpeed(smoothSpeed)
    {
    }
    FVector3 offset;
    float smoothSpeed;
};
struct PlayerComponent
{
    PlayerComponent(float moveSpeed) : moveSpeed(moveSpeed)
    {
    }

    float moveSpeed = 0.0f;
};
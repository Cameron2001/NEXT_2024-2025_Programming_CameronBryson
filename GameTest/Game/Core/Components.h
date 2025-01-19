#pragma once
#include "Entity.h"
#include "Game/Math/Vector3.h"
#include <Game/Math/Matrix3.h>
#include <Game/Math/Quaternion.h>
#include <Game/Math/Vector2.h>
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
        : Rotation(Quaternion::FromEulerAnglesXYZ(rotation)), Position(position), Scale(scale)
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

    bool isInitialized = false;
};

struct TextComponent
{
    TextComponent(std::string text, const FVector2 &position) : text(std::move(text)), position(position)
    {
    }
    std::string text;
    FVector2 position;
};
struct ModelComponent
{
    ModelComponent(std::string modelName) : modelName(std::move(modelName)), color({1.0f, 1.0f, 1.0f})
    {
    }
    ModelComponent(std::string modelName, const FVector3 &color) : modelName(std::move(modelName)), color(color)
    {
    }
    ModelComponent(std::string modelName, const FVector3 &color, int layer)
        : modelName(std::move(modelName)), color(color), layer(layer)
    {
    }

    std::string modelName;
    FVector3 color = {1.0f, 1.0f, 1.0f};
    int layer = 0;
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
    ColliderComponent(const ColliderType type, bool isDynamic, bool isTrigger, float elasticity, float staticFriction,
                      float dynamicFriction)
        : type(type), layer(LAYER_DEFAULT), mask(0xFFFFFFFF), elasticity(elasticity), staticFriction(staticFriction),
          dynamicFriction(dynamicFriction), isDynamic(isDynamic), isTrigger(isTrigger)
    {
    }
    ColliderType type;
    unsigned int layer;
    unsigned int mask;
    float elasticity = 1.0f;
    float staticFriction = 0.5f;
    float dynamicFriction = 0.3f;
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
    PlayerComponent(const FVector3 &spawnPoint) : spawnPoint(spawnPoint)
    {
    }
    FVector3 spawnPoint;
};
struct ArrowComponent
{
    ArrowComponent(Entity ball) : ball(ball)
    {
    }
    Entity ball = 0;
};
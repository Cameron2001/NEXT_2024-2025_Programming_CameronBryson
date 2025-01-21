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
    TransformComponent(const FVector3 &position, const FVector3 &rotation, const FVector3 &scale)
        : rotation(Quaternion::FromEulerAnglesXYZ(rotation)), position(position), scale(scale)
    {
    }

    Quaternion rotation;
    FVector3 position;
    FVector3 scale;
};
struct BoxBoundsComponent
{
    BoxBoundsComponent(const FVector3 &extents) : extents(extents)
    {
    }

    FVector3 extents;
};

struct SphereBoundsComponent
{
    SphereBoundsComponent(const float radius) : radius(radius)
    {
    }

    float radius;
};

struct RigidBodyComponent
{
    RigidBodyComponent() = default;
    RigidBodyComponent(const float mass, const float linearDrag, const float angularDrag)
        : inverseMass(1.0f/mass),linearDrag(linearDrag), angularDrag(angularDrag)
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
    TextComponent(const std::string &text, const FVector2 &position) : text(text), position(position)
    {
    }
    std::string text;
    FVector2 position;
};
struct ModelComponent
{
    ModelComponent(const std::string& modelName) : modelName(modelName), color({1.0f, 1.0f, 1.0f})
    {
    }
    ModelComponent(const std::string& modelName, const FVector3 &color) : modelName(modelName), color(color)
    {
    }

    std::string modelName;
    FVector3 color = {1.0f, 1.0f, 1.0f};
    bool enabled = true;

};
struct ParticleComponent
{
    ParticleComponent(const FVector2 &position, const FVector3& color, const float rotation, const FVector2 &linearVelocity,
                      const float angularVelocity, const float lifetime)
        : position(position),color(color), linearVelocity(linearVelocity), rotation(rotation), angularVelocity(angularVelocity),
          lifetime(lifetime)
    {
    }
    FVector2 position = {0, 0};
    FVector3 color = {1, 0, 0};
    FVector2 linearVelocity = {0, 0};
    float rotation = 0;
    float angularVelocity = 0;
    float lifetime = 0;
    float age = 0;
};
struct ColliderComponent
{
    ColliderComponent(const ColliderType type, const bool isDynamic)
        : type(type), layer(LAYER_DEFAULT), mask(0xFFFFFFFF), isDynamic(isDynamic)
    {
    }
    ColliderComponent(const ColliderType type, const bool isDynamic, const float elasticity, const float friction)
        : type(type), layer(LAYER_DEFAULT), mask(0xFFFFFFFF), elasticity(elasticity), isDynamic(isDynamic),
          friction(friction)
    {
    }
    ColliderType type;
    unsigned int layer;
    unsigned int mask;
    float elasticity = 1.0f;
    float friction = 0.5f;
    bool isDynamic = false;
};
struct CameraFollowComponent
{
    CameraFollowComponent(const FVector3 &offset, const float smoothSpeed) : offset(offset), smoothSpeed(smoothSpeed)
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
    ArrowComponent(const Entity ball) : ball(ball)
    {
    }
    Entity ball = 0;
};
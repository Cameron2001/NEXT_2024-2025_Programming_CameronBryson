#include "stdafx.h"
#include "PhysicsSystem.h"
#include <Engine/Core/Components.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/Registry.h>
#include <tuple>

PhysicsSystem::PhysicsSystem(Registry *registry)
{
    m_registry = registry;
}

void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(const float dt)
{
    auto view = m_registry->CreateView<TransformComponent, RigidBodyComponent>();

    for (auto &&entity : view)
    {
        auto &transform = std::get<1>(entity);
        auto &rigidBody = std::get<2>(entity);
        // Update velocities
        rigidBody.linearVelocity += rigidBody.linearAcceleration * dt;
        rigidBody.angularVelocity += rigidBody.angularAcceleration * dt;

        // Update transform
        transform.Position += rigidBody.linearVelocity * dt;
        transform.Rotation += rigidBody.angularVelocity * dt;

        // Reset accelerations
        rigidBody.linearAcceleration = FVector3{0, 0, 0};
        rigidBody.angularAcceleration = FVector3{0, 0, 0};
    }
}

void PhysicsSystem::Shutdown()
{
}

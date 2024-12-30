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

    view.ParallelForEach([this, dt](const auto &entityTuple) {
        auto &transform = std::get<1>(entityTuple);
        auto &rigidBody = std::get<2>(entityTuple);

        // Update velocities
        rigidBody.linearVelocity += rigidBody.linearAcceleration * dt;
        rigidBody.angularVelocity += rigidBody.angularAcceleration * dt;

        // Apply damping
        rigidBody.linearVelocity *= std::pow(1 - rigidBody.linearDrag, dt);
        rigidBody.angularVelocity *= std::pow(1 - rigidBody.angularDrag, dt);

        // Update transform
        transform.Position += rigidBody.linearVelocity * dt;
        transform.Rotation += rigidBody.angularVelocity * dt;

        // Reset accelerations
        rigidBody.linearAcceleration = FVector3{0.0f, 0.0f, 0.0f};
        rigidBody.angularAcceleration = FVector3{0.0f, 0.0f, 0.0f};
    });
}

void PhysicsSystem::Shutdown()
{
}

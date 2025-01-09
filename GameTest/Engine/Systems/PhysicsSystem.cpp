#include "stdafx.h"
#include "PhysicsSystem.h"
#include <Engine/Core/Components.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/Registry.h>
#include <tuple>

PhysicsSystem::PhysicsSystem(Registry *registry) : m_registry(registry), m_view(registry)
{
}

void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(const float dt)
{
    // Update the view to reflect any changes in entities or components
    m_view.Update();

    m_view.ParallelForEach([this, dt](Entity entity, TransformComponent &transform, RigidBodyComponent &rigidBody) {
        // Update velocities
        rigidBody.linearVelocity += rigidBody.linearAcceleration * dt;
        rigidBody.angularVelocity += rigidBody.angularAcceleration * dt;

        // Apply damping
        rigidBody.linearVelocity *= std::pow(1.0f - rigidBody.linearDrag, dt);
        rigidBody.angularVelocity *= std::pow(1.0f - rigidBody.angularDrag, dt);

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

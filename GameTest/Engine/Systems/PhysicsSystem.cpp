#include "stdafx.h"
#include "PhysicsSystem.h"
#include <cmath>
#include <Engine/Core/Components.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/IComponentArray.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>

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

        FVector3 linearAcceleration = rigidBody.accumulatedForce * rigidBody.inverseMass;
        FVector3 angularAcceleration = rigidBody.accumulatedTorque * rigidBody.inverseMass; // simplified

        rigidBody.linearVelocity += linearAcceleration * dt;
        rigidBody.angularVelocity += angularAcceleration * dt;

        // Apply damping
        rigidBody.linearVelocity *= std::pow(1.0f - rigidBody.linearDrag, dt);
        rigidBody.angularVelocity *= std::pow(1.0f - rigidBody.angularDrag, dt);

        // Update transform
        transform.Position += rigidBody.linearVelocity * dt;
        transform.Rotation += rigidBody.angularVelocity * dt;

        // Reset accelerations
        rigidBody.accumulatedForce = FVector3{0.0f, 0.0f, 0.0f};
        rigidBody.accumulatedTorque = FVector3{0.0f, 0.0f, 0.0f};
    });
}

void PhysicsSystem::Shutdown()
{
}

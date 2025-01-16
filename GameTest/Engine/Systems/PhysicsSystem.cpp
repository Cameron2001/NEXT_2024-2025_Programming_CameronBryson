#include "stdafx.h"
#include "PhysicsSystem.h"
#include <cmath>
#include <Engine/Core/Components.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/IComponentArray.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>

PhysicsSystem::PhysicsSystem(Registry *registry)
    : m_registry(registry), m_view(registry), m_boxView(registry), m_sphereView(registry)
{
}

void PhysicsSystem::Init()
{
    m_boxView.ParallelForEach(
        [this](Entity entity, TransformComponent &transform, BoxBoundsComponent &boxBounds,
               RigidBodyComponent &rigidBody) { ComputeBoxInverseInertiaTensor(transform, boxBounds, rigidBody); });
    m_sphereView.ParallelForEach([this](Entity entity, TransformComponent &transform,
                                        SphereBoundsComponent &sphereBounds, RigidBodyComponent &rigidBody) {
        ComputeSphereInverseInertiaTensor(transform, sphereBounds, rigidBody);
    });
}

void PhysicsSystem::Update(const float dt)
{
    // Update the view to reflect any changes in entities or components
    m_view.Update();
    m_boxView.Update();
    m_sphereView.Update();

    m_boxView.ParallelForEach([this](Entity entity, TransformComponent &transform, BoxBoundsComponent &boxBounds,
                                     RigidBodyComponent &rigidBody) {
        if (!rigidBody.isInitialized)
        {
            ComputeBoxInverseInertiaTensor(transform, boxBounds, rigidBody);
        }
    });
    m_sphereView.ParallelForEach([this](Entity entity, TransformComponent &transform,
                                        SphereBoundsComponent &sphereBounds, RigidBodyComponent &rigidBody) {
        if (!rigidBody.isInitialized)
        {
            ComputeSphereInverseInertiaTensor(transform, sphereBounds, rigidBody);
        }
    });
    m_view.ParallelForEach([this, dt](Entity entity, TransformComponent &transform, RigidBodyComponent &rigidBody) {
        // Update velocities

        FVector3 linearAcceleration = rigidBody.force * rigidBody.inverseMass;

        Matrix3 rotationMatrix = transform.Rotation.GetRotationMatrix3();
        Matrix3 worldInverseInertiaTensor =
            rotationMatrix * rigidBody.localInverseInertiaTensor * rotationMatrix.Transpose();
        FVector3 angularAcceleration = worldInverseInertiaTensor * rigidBody.torque;

        rigidBody.linearVelocity += linearAcceleration * dt;
        rigidBody.angularVelocity += angularAcceleration * dt;

        // Apply damping
        rigidBody.linearVelocity *= std::pow(1.0f - rigidBody.linearDrag, dt);
        rigidBody.angularVelocity *= std::pow(1.0f - rigidBody.angularDrag, dt);

        // Update transform
        transform.Position += rigidBody.linearVelocity * dt;
        transform.Rotation += rigidBody.angularVelocity * dt;

        // Reset accelerations
        rigidBody.force = FVector3{0.0f, 0.0f, 0.0f};
        rigidBody.torque = FVector3{0.0f, 0.0f, 0.0f};
    });
}

void PhysicsSystem::Shutdown()
{
}

void PhysicsSystem::ComputeBoxInverseInertiaTensor(const TransformComponent &transform,
                                                   const BoxBoundsComponent &boxBounds, RigidBodyComponent &rigidBody)
{
    float mass = 1.0f / rigidBody.inverseMass;
    FVector3 scaledBoxExtents = boxBounds.extents * transform.Scale;

    Matrix3 inertiaTensor;
    inertiaTensor.SetZero();
    float factor = mass / 12.0f;

    inertiaTensor.Set(0, 0,
                      factor * (scaledBoxExtents.y * scaledBoxExtents.y + scaledBoxExtents.z * scaledBoxExtents.z));
    inertiaTensor.Set(1, 1,
                      factor * (scaledBoxExtents.x * scaledBoxExtents.x + scaledBoxExtents.z * scaledBoxExtents.z));
    inertiaTensor.Set(2, 2,
                      factor * (scaledBoxExtents.x * scaledBoxExtents.x + scaledBoxExtents.y * scaledBoxExtents.y));

    rigidBody.localInverseInertiaTensor = inertiaTensor.Inverse();
    rigidBody.isInitialized = true;
}

void PhysicsSystem::ComputeSphereInverseInertiaTensor(const TransformComponent &tranform,
                                                      const SphereBoundsComponent &sphereBounds,
                                                      RigidBodyComponent &rigidBody)
{
    float mass = 1.0f / rigidBody.inverseMass;
    float scaledRadius = sphereBounds.radius * (tranform.Scale.x + tranform.Scale.y + tranform.Scale.z) / 3.0f;

    Matrix3 inertiaTensor;
    inertiaTensor.SetZero();

    float I = (2.0f / 5.0f) * mass * scaledRadius * scaledRadius;

    inertiaTensor.Set(0, 0, I);
    inertiaTensor.Set(1, 1, I);
    inertiaTensor.Set(2, 2, I);

    rigidBody.localInverseInertiaTensor = inertiaTensor.Inverse();
    rigidBody.isInitialized = true;
}

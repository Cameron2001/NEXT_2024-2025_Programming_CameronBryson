#include "stdafx.h"
#include "PhysicsSystem.h"
#include <cmath>
#include <Game/Core/Components.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/IComponentArray.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>
const FVector3 GRAVITY(0.0f, -9.81f, 0.0f);
//const FVector3 GRAVITY(0.0f, 0.0f, 0.0f);
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
    m_view.ParallelForEach([this, dt](Entity entity, TransformComponent &transform, RigidBodyComponent &rigidBody) {
        // Check if the entity is dynamic (inverseMass > 0)
        if (rigidBody.inverseMass > 0.0f)
        {
            // Apply gravity force: F = m * g => since inverseMass = 1/m, F = g / inverseMass
            rigidBody.force += GRAVITY * (1.0f / rigidBody.inverseMass);
        }
    });
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
    m_view.ParallelForEach([dt](Entity entity, TransformComponent &transform, RigidBodyComponent &rigidBody) {
        // Update velocities

        FVector3 linearAcceleration = rigidBody.force * rigidBody.inverseMass;

        Matrix3 rotationMatrix = transform.rotation.GetRotationMatrix3();
        Matrix3 worldInverseInertiaTensor =
            rotationMatrix * rigidBody.localInverseInertiaTensor * rotationMatrix.Transpose();
        FVector3 angularAcceleration = worldInverseInertiaTensor * rigidBody.torque;

        rigidBody.linearVelocity += linearAcceleration * dt;
        rigidBody.angularVelocity += angularAcceleration * dt;

        // Apply damping
        rigidBody.linearVelocity *= std::pow(1.0f - rigidBody.linearDrag, dt);
        rigidBody.angularVelocity *= std::pow(1.0f- rigidBody.angularDrag, dt);

        // Update transform
        transform.position += rigidBody.linearVelocity * dt;

        Quaternion omega(0.0f, rigidBody.angularVelocity.x, rigidBody.angularVelocity.y, rigidBody.angularVelocity.z);
        Quaternion deltaRotation = omega * transform.rotation * 0.5f * dt;

        transform.rotation = (transform.rotation + deltaRotation).Normalize();

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

    FVector3 fullBoxDimensions = boxBounds.extents * 2.0f * transform.scale;

    float Ixx =
        (mass / 12.0f) * (fullBoxDimensions.y * fullBoxDimensions.y + fullBoxDimensions.z * fullBoxDimensions.z);
    float Iyy =
        (mass / 12.0f) * (fullBoxDimensions.x * fullBoxDimensions.x + fullBoxDimensions.z * fullBoxDimensions.z);
    float Izz =
        (mass / 12.0f) * (fullBoxDimensions.x * fullBoxDimensions.x + fullBoxDimensions.y * fullBoxDimensions.y);

    Matrix3 inertiaTensor;
    inertiaTensor.SetZero();
    inertiaTensor.Set(0, 0, Ixx);
    inertiaTensor.Set(1, 1, Iyy);
    inertiaTensor.Set(2, 2, Izz);

    rigidBody.localInverseInertiaTensor = inertiaTensor.Inverse();
    rigidBody.isInitialized = true;
}

void PhysicsSystem::ComputeSphereInverseInertiaTensor(const TransformComponent &tranform,
                                                      const SphereBoundsComponent &sphereBounds,
                                                      RigidBodyComponent &rigidBody)
{
    float mass = 1.0f / rigidBody.inverseMass;
    float scaledRadius = sphereBounds.radius * (tranform.scale.x + tranform.scale.y + tranform.scale.z) / 3.0f;

    Matrix3 inertiaTensor;
    inertiaTensor.SetZero();

    float I = (2.0f / 5.0f) * mass * scaledRadius * scaledRadius;

    inertiaTensor.Set(0, 0, I);
    inertiaTensor.Set(1, 1, I);
    inertiaTensor.Set(2, 2, I);

    rigidBody.localInverseInertiaTensor = inertiaTensor.Inverse();
    rigidBody.isInitialized = true;
}

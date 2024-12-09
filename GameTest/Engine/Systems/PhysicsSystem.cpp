#include "stdafx.h"
#include "PhysicsSystem.h"
#include "Engine/Storage/View.h"

PhysicsSystem::PhysicsSystem(Registry* registry)
{
    m_registry = registry;
}


void PhysicsSystem::Init()
{
}

void PhysicsSystem::Update(const float dt)
{
    auto view = m_registry->CreateView<TransformComponent, RigidBodyComponent>();

    for (auto it = view.begin(); it != view.end(); ++it)
    {
        auto &transform = std::get<0>(*it);
        auto &rigidBody = std::get<1>(*it);
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
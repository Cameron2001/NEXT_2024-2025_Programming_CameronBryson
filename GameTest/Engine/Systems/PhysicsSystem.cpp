#include "stdafx.h"
#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{
}


void PhysicsSystem::Init(std::shared_ptr<ComponentArray<TransformComponent>> transformComponentArray,
                         std::shared_ptr<ComponentArray<RigidBodyComponent>> rigidbodyComponentArray)
{
    m_transformComponents = transformComponentArray;
    m_rigidBodyComponents = rigidbodyComponentArray;
}

void PhysicsSystem::Update(const float dt)
{
    auto transformComponents = m_transformComponents.lock();
    auto rigidBodyComponents = m_rigidBodyComponents.lock();
    if (!transformComponents || !rigidBodyComponents)
    {
        return;
    }
    auto IDs = transformComponents->GetEntityIntersection(rigidBodyComponents->GetEntities());
    for (auto ID : IDs)
    {
        auto &transform = transformComponents->GetComponent(ID);
        auto &rigidBody = rigidBodyComponents->GetComponent(ID);
        rigidBody.linearVelocity += rigidBody.linearAcceleration * dt;
        rigidBody.angularVelocity += rigidBody.angularAcceleration * dt;
        transform.Position += rigidBody.linearVelocity * dt;
        transform.Rotation += rigidBody.angularVelocity * dt;
        rigidBody.linearAcceleration = FVector3{0, 0, 0};
        rigidBody.angularAcceleration = FVector3{0, 0, 0};
    }
}

void PhysicsSystem::Shutdown()
{
}
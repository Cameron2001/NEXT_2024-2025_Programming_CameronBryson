#include "stdafx.h"
#include "CollisionSystem.h"
#include <Engine/Core/Components.h>

CollisionSystem::CollisionSystem(Registry *registry) : m_registry(registry)
{
}

void CollisionSystem::Init()
{
}

void CollisionSystem::Update(const float dt)
{
    // auto sphereView = m_registry->CreateView<TransformComponent, SphereBoundsComponent>();
    // auto boxView = m_registry->CreateView<TransformComponent, BoxBoundsComponent>();
    //  create octree

    // Get colliders with dynamic bodies
    // Query dynamic body in octree
    //  Check for collisions
}

void CollisionSystem::LateUpdate(float dt)
{
}

void CollisionSystem::Shutdown()
{
}

bool CollisionSystem::OOBvsOOB(Entity ID1, Entity ID2)
{
    return false;
}

bool CollisionSystem::SpherevsSphere(Entity ID1, Entity ID2)
{
    return false;
}

bool CollisionSystem::SpherevsOOB(Entity ID1, Entity ID2)
{
    return false;
}

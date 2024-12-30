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
    BuildOctree();
    DetectCollisions();

    // we should prefetch potential collisions
}

void CollisionSystem::LateUpdate(float dt)
{
}

void CollisionSystem::Shutdown()
{
}

void CollisionSystem::BuildOctree()
{
    FVector3 minPoint(-100.0f, -100.0f, -100.0f);
    FVector3 maxPoint(100.0f, 100.0f, 100.0f);
    BoundingBox3D sceneBounds(minPoint, maxPoint);
    m_octree = std::make_unique<Octree>(sceneBounds);
    auto boxView = m_registry->CreateView<TransformComponent, BoxBoundsComponent>();
    auto sphereView = m_registry->CreateView<TransformComponent, SphereBoundsComponent>();
    for (const auto &box : boxView)
    {
        auto entityID = std::get<0>(box);
        auto &transform = std::get<1>(box);
        auto &boxBounds = std::get<2>(box);
        m_octree->insert(boxBounds, transform, entityID);
    }
    for (const auto &sphere : sphereView)
    {
        auto entityID = std::get<0>(sphere);
        auto &transform = std::get<1>(sphere);
        auto &sphereBounds = std::get<2>(sphere);
        m_octree->insert(sphereBounds, transform, entityID);
    }
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

bool CollisionSystem::CanCollide(const ColliderComponent &collider1, const ColliderComponent &collider2)
{
    if (!collider1.isDynamic && !collider2.isDynamic)
        return false;

    bool collider1CollidesWithCollider2 = (collider1.mask & collider2.layer) != 0;
    bool collider2CollidesWithCollider1 = (collider2.mask & collider1.layer) != 0;

    return collider1CollidesWithCollider2 && collider2CollidesWithCollider1;
}

void CollisionSystem::DetectCollisions()
{
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    m_octree->getPotentialCollisions(potentialCollisions);
    auto &transforms = m_registry->GetComponentArray<TransformComponent>();
    auto &boxBounds = m_registry->GetComponentArray<BoxBoundsComponent>();
    auto &sphereBounds = m_registry->GetComponentArray<SphereBoundsComponent>();
    auto &colliders = m_registry->GetComponentArray<ColliderComponent>();
    std::vector<std::pair<unsigned int, unsigned int>> filteredCollisions;
    for (const auto &collision : potentialCollisions)
    {
        auto entityID1 = collision.first;
        auto entityID2 = collision.second;
        auto &collider1 = colliders.GetComponent(entityID1);
        auto &collider2 = colliders.GetComponent(entityID2);
        if (CanCollide(collider1, collider2))
        {
            filteredCollisions.emplace_back(collision);
        }
    }

    for (const auto &collision : filteredCollisions)
    {
        auto entityID1 = collision.first;
        auto entityID2 = collision.second;
        auto &transform1 = transforms.GetComponent(entityID1);
        auto &transform2 = transforms.GetComponent(entityID2);
    }
}

void CollisionSystem::ResolveCollisions()
{
    for (const auto &collision : m_collisions)
    {
    }
}

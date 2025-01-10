#include "stdafx.h"
#include "CollisionSystem.h"
#include <Engine/Core/Components.h>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>
#include <Engine/Math/BoundingBox.h>
#include <Engine/Math/Octree.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/IComponentArray.h>
#include <Engine/Storage/Registry.h>

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
    ResolveCollisions();

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
    // Abartary bounds for now
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
        m_octree->Insert(boxBounds, transform, entityID);
    }
    for (const auto &sphere : sphereView)
    {
        auto entityID = std::get<0>(sphere);
        auto &transform = std::get<1>(sphere);
        auto &sphereBounds = std::get<2>(sphere);
        m_octree->Insert(sphereBounds, transform, entityID);
    }
}

bool CollisionSystem::TestAxisOverlap(const FVector3 &axis, const BoxBoundsComponent &box1, const Matrix4 &rotation1,
                                      const BoxBoundsComponent &box2, const Matrix4 &rotation2,
                                      const FVector3 &translation, float &minimalPenetration,
                                      FVector3 &collisionNormal) const
{
    if (axis.LengthSquared() < 0.00001f)
    {
        return true;
    }
    return false;
}

bool CollisionSystem::OOBvsOOB(Entity ID1, Entity ID2)
{
    // Append to m_collisions
    auto &transform1 = m_registry->GetComponent<TransformComponent>(ID1);
    auto &transform2 = m_registry->GetComponent<TransformComponent>(ID2);

    auto &box1 = m_registry->GetComponent<BoxBoundsComponent>(ID1);
    auto &box2 = m_registry->GetComponent<BoxBoundsComponent>(ID2);

    const Matrix4 rotation1 = Matrix4::CreateEulerAngleMatrixXYZ(transform1.Rotation);
    const Matrix4 rotation2 = Matrix4::CreateEulerAngleMatrixXYZ(transform2.Rotation);

    return false;
}

bool CollisionSystem::SpherevsSphere(Entity ID1, Entity ID2)
{
    // Append to m_collisions
    auto &transform1 = m_registry->GetComponent<TransformComponent>(ID1);
    auto &transform2 = m_registry->GetComponent<TransformComponent>(ID2);

    auto &sphere1 = m_registry->GetComponent<SphereBoundsComponent>(ID1);
    auto &sphere2 = m_registry->GetComponent<SphereBoundsComponent>(ID2);
    return false;
}

bool CollisionSystem::SpherevsOOB(Entity ID1, Entity ID2)
{
    // Append to m_collisions
    auto &sphereTransform = m_registry->GetComponent<TransformComponent>(ID1);
    auto &boxTransform = m_registry->GetComponent<TransformComponent>(ID2);

    auto &sphereBounds = m_registry->GetComponent<SphereBoundsComponent>(ID1);
    auto &boxBounds = m_registry->GetComponent<BoxBoundsComponent>(ID2);

    const Matrix4 rotation = Matrix4::CreateEulerAngleMatrixXYZ(boxTransform.Rotation);
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
    m_collisions.clear(); // Clear previous collisions

    // Retrieve potential collisions from the octree
    std::vector<std::pair<unsigned int, unsigned int>> potentialCollisions;
    m_octree->GetPotentialCollisions(potentialCollisions);

    auto &colliders = m_registry->GetComponentArray<ColliderComponent>();

    // Iterate over potential collision pairs
    for (const auto &collisionPair : potentialCollisions)
    {
        unsigned int entityID1 = collisionPair.first;
        unsigned int entityID2 = collisionPair.second;

        const auto &collider1 = colliders.GetComponent(entityID1);
        const auto &collider2 = colliders.GetComponent(entityID2);

        // Check if the colliders can collide based on their properties
        if (!CanCollide(collider1, collider2))
            continue;

        bool collisionDetected = false;

        // Determine collider types and call the appropriate collision detection function
        if (collider1.type == ColliderType::Sphere && collider2.type == ColliderType::Sphere)
        {
            collisionDetected = SpherevsSphere(entityID1, entityID2);
        }
        else if (collider1.type == ColliderType::Box && collider2.type == ColliderType::Box)
        {
            collisionDetected = OOBvsOOB(entityID1, entityID2);
        }
        else if (collider1.type == ColliderType::Sphere && collider2.type == ColliderType::Box)
        {
            collisionDetected = SpherevsOOB(entityID1, entityID2);
        }
        else if (collider1.type == ColliderType::Box && collider2.type == ColliderType::Sphere)
        {
            // Swap entities to match SpherevsOOB parameter expectations
            collisionDetected = SpherevsOOB(entityID2, entityID1);
        }
    }
}

// Do we want to refer the resolution? or do we want to resolve them as they are found
void CollisionSystem::ResolveCollisions()
{
    for (const auto &collision : m_collisions)
    {
        unsigned int entityID1 = collision.ID1;
        unsigned int entityID2 = collision.ID2;

        auto &transform1 = m_registry->GetComponent<TransformComponent>(entityID1);
        auto &transform2 = m_registry->GetComponent<TransformComponent>(entityID2);

        bool isDynamic1 = m_registry->HasComponent<RigidBodyComponent>(entityID1);
        bool isDynamic2 = m_registry->HasComponent<RigidBodyComponent>(entityID2);
    }
}

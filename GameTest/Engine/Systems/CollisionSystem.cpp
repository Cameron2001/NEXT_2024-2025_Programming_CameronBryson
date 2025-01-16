#include "stdafx.h"
#include "CollisionSystem.h"
#include <cmath>
#include <Engine/Core/Collision.h>
#include <Engine/Core/Components.h>
#include <Engine/Math/BoundingBox.h>
#include <Engine/Math/Matrix3.h>
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Octree.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/IComponentArray.h>
#include <Engine/Storage/Registry.h>
#include <limits>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

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
    const FVector3 minPoint(-100.0f, -100.0f, -100.0f);
    const FVector3 maxPoint(100.0f, 100.0f, 100.0f);
    const BoundingBox3D sceneBounds(minPoint, maxPoint);
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

bool CollisionSystem::TestAxisOverlap(const FVector3 &axis, const BoxBoundsComponent &box1, const FVector3 &scale1,
                                      const Matrix4 &rotation1, const BoxBoundsComponent &box2, const FVector3 &scale2,
                                      const Matrix4 &rotation2, const FVector3 &translation, float &minimalPenetration,
                                      FVector3 &collisionNormal) const
{
    const FVector3 scaledBox1Extents = box1.extents * scale1;
    const FVector3 scaledBox2Extents = box2.extents * scale2;
    const float projection1 = scaledBox1Extents.x * fabs(rotation1.GetRight().Dot(axis)) +
                              scaledBox1Extents.y * fabs(rotation1.GetUp().Dot(axis)) +
                              scaledBox1Extents.z * fabs(rotation1.GetForward().Dot(axis));
    const float projection2 = scaledBox2Extents.x * fabs(rotation2.GetRight().Dot(axis)) +
                              scaledBox2Extents.y * fabs(rotation2.GetUp().Dot(axis)) +
                              scaledBox2Extents.z * fabs(rotation2.GetForward().Dot(axis));

    const float projectionTranslation = translation.Dot(axis);

    // Calculate overlap
    const float overlap = (projection1 + projection2) - fabs(projectionTranslation);

    if (overlap < 0.0f)
    {
        // Separating axis found
        return false;
    }

    // Update minimal penetration and collision normal
    if (overlap < minimalPenetration)
    {
        minimalPenetration = overlap;
        collisionNormal = axis;
    }

    return true;
}

bool CollisionSystem::OOBvsOOB(Entity ID1, Entity ID2)
{
    auto &transform1 = m_registry->GetComponent<TransformComponent>(ID1);
    auto &transform2 = m_registry->GetComponent<TransformComponent>(ID2);

    auto &box1 = m_registry->GetComponent<BoxBoundsComponent>(ID1);
    auto &box2 = m_registry->GetComponent<BoxBoundsComponent>(ID2);

    const Matrix4 rotation1 = transform1.Rotation.GetRotationMatrix4();
    const Matrix4 rotation2 = transform2.Rotation.GetRotationMatrix4();

    const FVector3 translation = transform2.Position - transform1.Position;

    float minimalPenetration = std::numeric_limits<float>::max();
    FVector3 collisionNormal;

    std::vector<FVector3> axes;
    axes.emplace_back(rotation1.GetRight().Normalize());
    axes.emplace_back(rotation1.GetUp().Normalize());
    axes.emplace_back(rotation1.GetForward().Normalize());

    axes.emplace_back(rotation2.GetRight().Normalize());
    axes.emplace_back(rotation2.GetUp().Normalize());
    axes.emplace_back(rotation2.GetForward().Normalize());

    // Add cross product axes
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 3; j < 6; ++j)
        {
            const FVector3 axis = axes[i].Cross(axes[j]).Normalize();
            if (axis.LengthSquared() > 0.00001f)
            {
                axes.emplace_back(axis);
            }
        }
    }

    for (const auto &axis : axes)
    {
        if (!TestAxisOverlap(axis, box1, transform1.Scale, rotation1, box2, transform2.Scale, rotation2, translation,
                             minimalPenetration, collisionNormal))
        {
            return false;
        }
    }
    FVector3 contactPoint = transform1.Position + collisionNormal * (minimalPenetration / 2.0f);
    Collision collision(ID1, ID2, contactPoint, collisionNormal, minimalPenetration);
    const FVector3 direction = (transform2.Position - transform1.Position).Normalize();
    if (collision.normal.Dot(direction) < 0)
        collision.normal = collision.normal * -1.0f;

    m_collisions.emplace_back(collision);

    return true;
}

bool CollisionSystem::SpherevsSphere(Entity ID1, Entity ID2)
{
    auto &transform1 = m_registry->GetComponent<TransformComponent>(ID1);
    auto &transform2 = m_registry->GetComponent<TransformComponent>(ID2);

    auto &sphere1 = m_registry->GetComponent<SphereBoundsComponent>(ID1);
    auto &sphere2 = m_registry->GetComponent<SphereBoundsComponent>(ID2);

    const float radius1 = sphere1.radius * (transform1.Scale.x + transform1.Scale.y + transform1.Scale.z) / 3;
    const float radius2 = sphere2.radius * (transform2.Scale.x + transform2.Scale.y + transform2.Scale.z) / 3;

    const FVector3 delta = transform2.Position - transform1.Position;
    const float distanceSquared = delta.LengthSquared();
    const float radiusSum = radius1 + radius2;

    if (distanceSquared < radiusSum * radiusSum)
    {
        const float distance = sqrt(distanceSquared);
        const float penetration = radiusSum - distance;

        const FVector3 collisionNormal = (distance != 0.0f) ? (delta / distance) : FVector3(1.0f, 0.0f, 0.0f);
        FVector3 contactPoint = transform1.Position + collisionNormal * radius1;
        m_collisions.emplace_back(ID1, ID2, contactPoint, collisionNormal, penetration);
        return true;
    }

    return false;
}

bool CollisionSystem::SpherevsOOB(Entity ID1, Entity ID2)
{
    auto &sphereTransform = m_registry->GetComponent<TransformComponent>(ID1);
    auto &boxTransform = m_registry->GetComponent<TransformComponent>(ID2);

    auto &sphereBounds = m_registry->GetComponent<SphereBoundsComponent>(ID1);
    auto &boxBounds = m_registry->GetComponent<BoxBoundsComponent>(ID2);

    // Compute sphere radius including scaling
    const float radius =
        sphereBounds.radius * (sphereTransform.Scale.x + sphereTransform.Scale.y + sphereTransform.Scale.z) / 3.0f;

    // Compute box extents including scaling
    const FVector3 scaledBoxExtents = boxBounds.extents * boxTransform.Scale;

    // Get OBB's orientation axes
    const Matrix4 boxRotationMatrix = boxTransform.Rotation.GetRotationMatrix4();
    const FVector3 boxRight = boxRotationMatrix.GetRight().Normalize();
    const FVector3 boxUp = boxRotationMatrix.GetUp().Normalize();
    const FVector3 boxForward = boxRotationMatrix.GetForward().Normalize();

    // Compute vector from box center to sphere center
    const FVector3 delta = sphereTransform.Position - boxTransform.Position;

    // Project delta onto OBB's local axes to get the sphere center in OBB's local space
    float deltaLocalX = delta.Dot(boxRight);
    float deltaLocalY = delta.Dot(boxUp);
    float deltaLocalZ = delta.Dot(boxForward);

    // Clamp the projected point to the extents of the OBB to find the closest point
    float closestPointLocalX = std::max(-scaledBoxExtents.x, std::min(deltaLocalX, scaledBoxExtents.x));
    float closestPointLocalY = std::max(-scaledBoxExtents.y, std::min(deltaLocalY, scaledBoxExtents.y));
    float closestPointLocalZ = std::max(-scaledBoxExtents.z, std::min(deltaLocalZ, scaledBoxExtents.z));

    // Transform the closest point back to world space
    FVector3 closestPointWorld = boxTransform.Position + boxRight * closestPointLocalX + boxUp * closestPointLocalY +
                                 boxForward * closestPointLocalZ;

    // Compute the vector from the closest point to the sphere center
    FVector3 difference = sphereTransform.Position - closestPointWorld;

    float distanceSquared = difference.LengthSquared();

    if (distanceSquared > radius * radius + 0.00001f)
    {
        // No collision
        return false;
    }

    // Collision detected
    FVector3 collisionNormal;
    float penetration;
    FVector3 contactPoint;

    if (distanceSquared > 0.00001f)
    {
        // Sphere center is outside the OBB
        float distance = std::sqrt(distanceSquared);
        collisionNormal = difference / distance;
        penetration = radius - distance;

        // Contact point is the closest point on the OBB
        contactPoint = closestPointWorld;
    }
    else
    {
        // Sphere center is inside the OBB
        // Find the axis of least penetration
        float minPenetration = radius;
        FVector3 localNormal;

        float penetrationX = scaledBoxExtents.x - std::abs(deltaLocalX);
        if (penetrationX < minPenetration)
        {
            minPenetration = penetrationX;
            localNormal = boxRight * ((deltaLocalX < 0) ? -1.0f : 1.0f);
        }

        float penetrationY = scaledBoxExtents.y - std::abs(deltaLocalY);
        if (penetrationY < minPenetration)
        {
            minPenetration = penetrationY;
            localNormal = boxUp * ((deltaLocalY < 0) ? -1.0f : 1.0f);
        }

        float penetrationZ = scaledBoxExtents.z - std::abs(deltaLocalZ);
        if (penetrationZ < minPenetration)
        {
            minPenetration = penetrationZ;
            localNormal = boxForward * ((deltaLocalZ < 0) ? -1.0f : 1.0f);
        }

        collisionNormal = localNormal.Normalize();
        penetration = minPenetration + radius;

        // Contact point is the sphere center adjusted by the collision normal scaled by the sphere radius
        contactPoint = sphereTransform.Position + collisionNormal * radius;
    }

    // Adjust collision normal to point from sphere to box
    const FVector3 direction = (boxTransform.Position - sphereTransform.Position).Normalize();
    if (collisionNormal.Dot(direction) < 0)
        collisionNormal = collisionNormal * -1.0f;

    // Ensure penetration is positive
    if (penetration <= 0.0f)
    {
        return false;
    }

    // Record the collision
    m_collisions.emplace_back(ID1, ID2, contactPoint, collisionNormal, penetration);

    return true;
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

void CollisionSystem::ResolveCollisions()
{
    for (const auto &collision : m_collisions)
    {
        unsigned int entityID1 = collision.ID1;
        unsigned int entityID2 = collision.ID2;

        bool hasRigidBody1 = m_registry->HasComponent<RigidBodyComponent>(entityID1);
        bool hasRigidBody2 = m_registry->HasComponent<RigidBodyComponent>(entityID2);

        if (!hasRigidBody1 && !hasRigidBody2)
            continue; // No physics to resolve

        TransformComponent &transform1 = m_registry->GetComponent<TransformComponent>(entityID1);
        TransformComponent &transform2 = m_registry->GetComponent<TransformComponent>(entityID2);

        RigidBodyComponent *rb1 = hasRigidBody1 ? &m_registry->GetComponent<RigidBodyComponent>(entityID1) : nullptr;
        RigidBodyComponent *rb2 = hasRigidBody2 ? &m_registry->GetComponent<RigidBodyComponent>(entityID2) : nullptr;

        FVector3 velocity1 = hasRigidBody1 ? rb1->linearVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 velocity2 = hasRigidBody2 ? rb2->linearVelocity : FVector3(0.0f, 0.0f, 0.0f);

        FVector3 angularVelocity1 = hasRigidBody1 ? rb1->angularVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 angularVelocity2 = hasRigidBody2 ? rb2->angularVelocity : FVector3(0.0f, 0.0f, 0.0f);

        FVector3 r1 = collision.contactPoint - transform1.Position;
        FVector3 r2 = collision.contactPoint - transform2.Position;

        FVector3 relativeVelocity = (velocity2 + angularVelocity2.Cross(r2)) - (velocity1 + angularVelocity1.Cross(r1));

        float velocityAlongNormal = relativeVelocity.Dot(collision.normal);

        if (velocityAlongNormal > 0)
            continue;

        float restitution = 1.0f;
        if (rb1 && rb2)
            restitution = rb1->elasticity * rb2->elasticity;
        else if (rb1)
            restitution = rb1->elasticity;
        else if (rb2)
            restitution = rb2->elasticity;

        float inverseMass1 = hasRigidBody1 ? rb1->inverseMass : 0.0f;
        float inverseMass2 = hasRigidBody2 ? rb2->inverseMass : 0.0f;

        Matrix3 inverseInertiaTensor1 = hasRigidBody1 ? rb1->localInverseInertiaTensor : Matrix3();
        if (!hasRigidBody1)
            inverseInertiaTensor1.SetZero();
        Matrix3 inverseInertiaTensor2 = hasRigidBody2 ? rb2->localInverseInertiaTensor : Matrix3();
        if (!hasRigidBody2)
            inverseInertiaTensor2.SetZero();

        FVector3 r1CrossN = r1.Cross(collision.normal);
        FVector3 r2CrossN = r2.Cross(collision.normal);

        float angularComponent1 =
            hasRigidBody1 ? (inverseInertiaTensor1 * r1CrossN).Cross(r1).Dot(collision.normal) : 0.0f;
        float angularComponent2 =
            hasRigidBody2 ? (inverseInertiaTensor2 * r2CrossN).Cross(r2).Dot(collision.normal) : 0.0f;

        float impulseScalar = -(1.0f + restitution) * velocityAlongNormal;
        impulseScalar /= (inverseMass1 + inverseMass2 + angularComponent1 + angularComponent2);

        FVector3 impulse = collision.normal * impulseScalar;

        if (rb1)
        {
            rb1->linearVelocity -= impulse * inverseMass1;
            rb1->angularVelocity += inverseInertiaTensor1 * r1.Cross(impulse);
        }
        if (rb2)
        {
            rb2->linearVelocity += impulse * inverseMass2;
            rb2->angularVelocity -= inverseInertiaTensor2 * r2.Cross(impulse);
        }
    }
}

#include "stdafx.h"
#include "CollisionSystem.h"
#include <cmath>
#include <cstdlib>
#include <Game/Core/Collision.h>
#include <Game/Core/Components.h>
#include <Game/Core/Entity.h>
#include <Game/Managers/EventManager.h>
#include <Game/Math/BoundingBox.h>
#include <Game/Math/Matrix3.h>
#include <Game/Math/Matrix4.h>
#include <Game/Math/Octree.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>
#include <limits>
#include <memory>
#include <ppl.h>
#include <utility>
#include <vector>
#include "Game/Math/MathUtil.h"

CollisionSystem::CollisionSystem(Registry *registry, EventManager *eventManager)
    : m_registry(registry), m_eventManager(eventManager), m_boxView(registry), m_sphereView(registry),
      m_dyanmicBoxView(registry), m_dynamicSphereView(registry)
{
}

void CollisionSystem::Init()
{
    
}

void CollisionSystem::LateInit()
{
    BuildOctree();
}

void CollisionSystem::Update(const float dt)
{
    UpdateOctree();
    DetectCollisions();
    ResolveCollisions(dt);

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
    const FVector3 minPoint(-100.0f, -100.0f, -1000.0f);
    const FVector3 maxPoint(100.0f, 100.0f, 10.0f);
    const BoundingBox3D sceneBounds(minPoint, maxPoint);
    m_octree = std::make_unique<Octree>(sceneBounds);
    m_boxView.Update();
    m_sphereView.Update();
    m_boxView.ForEach([this](const Entity entity, const TransformComponent &transform, const BoxBoundsComponent &boxBounds,
                             const ColliderComponent &collider) { m_octree->Insert(boxBounds, transform, entity,collider.isDynamic); });

    // Insert sphere colliders into the octree
    m_sphereView.ForEach([this](const Entity entity, const TransformComponent &transform, const SphereBoundsComponent &sphereBounds,
                                const ColliderComponent &collider) { m_octree->Insert(sphereBounds, transform, entity,collider.isDynamic); });
}

void CollisionSystem::UpdateOctree()
{
    if (!m_octree)
    {
        return;
    }
    m_octree->ClearDynamicColliders();
    m_dyanmicBoxView.Update();
    m_dynamicSphereView.Update();
    m_dyanmicBoxView.ForEach(
        [this](const Entity entity, const TransformComponent &transform, const BoxBoundsComponent &boxBounds,
               const ColliderComponent &collider, RigidBodyComponent&) { m_octree->Insert(boxBounds, transform, entity, collider.isDynamic); });
    m_dynamicSphereView.ForEach(
        [this](const Entity entity, const TransformComponent &transform, const SphereBoundsComponent &sphereBounds,
               const ColliderComponent &collider, RigidBodyComponent &) { m_octree->Insert(sphereBounds, transform, entity, collider.isDynamic); });
}

bool CollisionSystem::TestAxisOverlap(const FVector3 &axis, const BoxBoundsComponent &box1, const FVector3 &scale1,
                                      const Matrix4 &rotation1, const BoxBoundsComponent &box2, const FVector3 &scale2,
                                      const Matrix4 &rotation2, const FVector3 &translation, float &minimalPenetration,
                                      FVector3 &collisionNormal)
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
    const auto &transform1 = m_registry->GetComponent<TransformComponent>(ID1);
    const auto &transform2 = m_registry->GetComponent<TransformComponent>(ID2);

    const auto &box1 = m_registry->GetComponent<BoxBoundsComponent>(ID1);
    const auto &box2 = m_registry->GetComponent<BoxBoundsComponent>(ID2);

    const Matrix4 rotation1 = transform1.rotation.GetRotationMatrix4();
    const Matrix4 rotation2 = transform2.rotation.GetRotationMatrix4();

    const FVector3 translation = transform2.position - transform1.position;

    float minimalPenetration = (std::numeric_limits<float>::max)();
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
        if (!TestAxisOverlap(axis, box1, transform1.scale, rotation1, box2, transform2.scale, rotation2, translation,
                             minimalPenetration, collisionNormal))
        {
            return false;
        }
    }
    const FVector3 contactPoint = transform1.position + collisionNormal * (minimalPenetration / 2.0f);
    Collision collision(ID1, ID2, contactPoint, collisionNormal, minimalPenetration);
    const FVector3 direction = (transform2.position - transform1.position).Normalize();
    if (collision.normal.Dot(direction) < 0)
        collision.normal = collision.normal * -1.0f;

    m_threadCollisions.local().emplace_back(collision);
    // m_collisions.emplace_back(collision);

    return true;
}

bool CollisionSystem::SpherevsSphere(Entity ID1, Entity ID2)
{
    const auto &transform1 = m_registry->GetComponent<TransformComponent>(ID1);
    const auto &transform2 = m_registry->GetComponent<TransformComponent>(ID2);

    const auto &sphere1 = m_registry->GetComponent<SphereBoundsComponent>(ID1);
    const auto &sphere2 = m_registry->GetComponent<SphereBoundsComponent>(ID2);

    const float radius1 = sphere1.radius * (transform1.scale.x + transform1.scale.y + transform1.scale.z) / 3;
    const float radius2 = sphere2.radius * (transform2.scale.x + transform2.scale.y + transform2.scale.z) / 3;

    const FVector3 delta = transform2.position - transform1.position;
    const float distanceSquared = delta.LengthSquared();
    const float radiusSum = radius1 + radius2;

    if (distanceSquared < radiusSum * radiusSum)
    {
        const float distance = sqrt(distanceSquared);
        const float penetration = radiusSum - distance;

        const FVector3 collisionNormal = (distance != 0.0f) ? (delta / distance) : FVector3(1.0f, 0.0f, 0.0f);
        FVector3 contactPoint = transform1.position + collisionNormal * radius1;
        m_threadCollisions.local().emplace_back(ID1, ID2, contactPoint, collisionNormal, penetration);
        return true;
    }

    return false;
}

bool CollisionSystem::SpherevsOOB(Entity ID1, Entity ID2)
{
    const auto &sphereTransform = m_registry->GetComponent<TransformComponent>(ID1);
    const auto &boxTransform = m_registry->GetComponent<TransformComponent>(ID2);

    const auto &sphereBounds = m_registry->GetComponent<SphereBoundsComponent>(ID1);
    const auto &boxBounds = m_registry->GetComponent<BoxBoundsComponent>(ID2);

    const float radius =
        sphereBounds.radius * (sphereTransform.scale.x + sphereTransform.scale.y + sphereTransform.scale.z) / 3.0f;

    const FVector3 scaledBoxExtents = boxBounds.extents * boxTransform.scale;

    const Matrix4 boxRotationMatrix = boxTransform.rotation.GetRotationMatrix4();
    const FVector3 boxRight = boxRotationMatrix.GetRight().Normalize();
    const FVector3 boxUp = boxRotationMatrix.GetUp().Normalize();
    const FVector3 boxForward = boxRotationMatrix.GetForward().Normalize();

    // Compute vector from box center to sphere center
    const FVector3 delta = sphereTransform.position - boxTransform.position;

    // Project delta onto OBB's local axes to get the sphere center in OBB's local space
    const float deltaLocalX = delta.Dot(boxRight);
    const float deltaLocalY = delta.Dot(boxUp);
    const float deltaLocalZ = delta.Dot(boxForward);

    // Clamp the projected point to the extents of the OBB to find the closest point
    const float closestPointLocalX = (std::max)(-scaledBoxExtents.x, (std::min)(deltaLocalX, scaledBoxExtents.x));
    const float closestPointLocalY = (std::max)(-scaledBoxExtents.y, (std::min)(deltaLocalY, scaledBoxExtents.y));
    const float closestPointLocalZ = (std::max)(-scaledBoxExtents.z, (std::min)(deltaLocalZ, scaledBoxExtents.z));

    // Transform the closest point back to world space
    const FVector3 closestPointWorld = boxTransform.position + boxRight * closestPointLocalX + boxUp * closestPointLocalY +
                                 boxForward * closestPointLocalZ;

    // Compute the vector from the closest point to the sphere center
    const FVector3 difference = sphereTransform.position - closestPointWorld;

    const float distanceSquared = difference.LengthSquared();

    if (distanceSquared > radius * radius + 0.00001f)
    {
        // No collision
        return false;
    }

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
        contactPoint = sphereTransform.position + collisionNormal * radius;
    }

    // Adjust collision normal to point from sphere to box
    const FVector3 direction = (boxTransform.position - sphereTransform.position).Normalize();
    if (collisionNormal.Dot(direction) < 0)
        collisionNormal = collisionNormal * -1.0f;

    // Ensure penetration is positive
    if (penetration <= 0.0f)
    {
        return false;
    }

    m_threadCollisions.local().emplace_back(ID1, ID2, contactPoint, collisionNormal, penetration);

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
    m_collisions.clear();
    m_potentialCollisions.clear();
    m_threadCollisions.clear();

    m_octree->GetPotentialCollisions(m_potentialCollisions);

    auto &colliders = m_registry->GetComponentArray<ColliderComponent>();

    // Iterate over potential collision pairs
    concurrency::parallel_for_each(
        m_potentialCollisions.begin(), m_potentialCollisions.end(),
        [&](const std::pair<unsigned int, unsigned int> &collisionPair) {
            const unsigned int entityID1 = collisionPair.first;
            const unsigned int entityID2 = collisionPair.second;

            const auto &collider1 = colliders.GetComponent(entityID1);
            const auto &collider2 = colliders.GetComponent(entityID2);
            if (!CanCollide(collider1, collider2))
                return;
            bool collisionDetected = false;
            if (collider1.type == ColliderType::Sphere && collider2.type == ColliderType::Sphere)
            {
                if (SpherevsSphere(entityID1, entityID2))
                    collisionDetected = true;
            }
            else if (collider1.type == ColliderType::Box && collider2.type == ColliderType::Box)
            {
                if (OOBvsOOB(entityID1, entityID2))
                    collisionDetected = true;
            }
            else if (collider1.type == ColliderType::Sphere && collider2.type == ColliderType::Box)
            {
                if (SpherevsOOB(entityID1, entityID2))
                    collisionDetected = true;
            }
            else if (collider1.type == ColliderType::Box && collider2.type == ColliderType::Sphere)
            {
                if (SpherevsOOB(entityID2, entityID1))
                    collisionDetected = true;
            }
            if (collisionDetected)
            {
                m_eventManager->Notify("CollisionEvent", entityID1, entityID2);
            }
        });
    m_threadCollisions.combine_each([&](const std::vector<Collision> &threadCollisions) {
        m_collisions.insert(m_collisions.end(), threadCollisions.begin(), threadCollisions.end());
    });
}

void CollisionSystem::ResolveCollisions(float dt)
{
    const float percent = 0.8f;                      // Penetration percentage to correct
    const float slop = 0.001f;                       // Penetration allowance
    const float restitutionVelocityThreshold = 0.1f; // Threshold to ignore small velocities

    for (auto &collision : m_collisions)
    {
        unsigned int entityID1 = collision.ID1;
        unsigned int entityID2 = collision.ID2;

        bool hasRigidBody1 = m_registry->HasComponent<RigidBodyComponent>(entityID1);
        bool hasRigidBody2 = m_registry->HasComponent<RigidBodyComponent>(entityID2);

        if (!hasRigidBody1 && !hasRigidBody2)
            continue;

        // Retrieve components
        TransformComponent &transform1 = m_registry->GetComponent<TransformComponent>(entityID1);
        TransformComponent &transform2 = m_registry->GetComponent<TransformComponent>(entityID2);

        RigidBodyComponent *rb1 = hasRigidBody1 ? &m_registry->GetComponent<RigidBodyComponent>(entityID1) : nullptr;
        RigidBodyComponent *rb2 = hasRigidBody2 ? &m_registry->GetComponent<RigidBodyComponent>(entityID2) : nullptr;

        ColliderComponent &collider1 = m_registry->GetComponent<ColliderComponent>(entityID1);
        ColliderComponent &collider2 = m_registry->GetComponent<ColliderComponent>(entityID2);

        FVector3 r1 = collision.contactPoint - transform1.position;
        FVector3 r2 = collision.contactPoint - transform2.position;

        FVector3 direction = (transform2.position - transform1.position).Normalize();
        if (collision.normal.Dot(direction) < 0.0f)
        {
            collision.normal = -collision.normal;
        }

        FVector3 v1 = rb1 ? (rb1->linearVelocity + rb1->angularVelocity.Cross(r1)) : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 v2 = rb2 ? (rb2->linearVelocity + rb2->angularVelocity.Cross(r2)) : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 relativeVel = v2 - v1;
        float relVelAlongNormal = relativeVel.Dot(collision.normal);

        if (relVelAlongNormal > restitutionVelocityThreshold)
            continue;

        float e = collider1.elasticity * collider2.elasticity;

        Matrix3 rotationMatrix1 = transform1.rotation.GetRotationMatrix3();
        Matrix3 rotationMatrix2 = transform2.rotation.GetRotationMatrix3();

        Matrix3 worldInvInertia1 =
            rb1 ? (rotationMatrix1 * rb1->localInverseInertiaTensor * rotationMatrix1.Transpose())
                : Matrix3().SetZero();
        Matrix3 worldInvInertia2 =
            rb2 ? (rotationMatrix2 * rb2->localInverseInertiaTensor * rotationMatrix2.Transpose())
                : Matrix3().SetZero();

        FVector3 r1CrossN = r1.Cross(collision.normal);
        FVector3 r2CrossN = r2.Cross(collision.normal);

        float denom = (rb1 ? rb1->inverseMass : 0.0f) + (rb2 ? rb2->inverseMass : 0.0f) +
                      r1CrossN.Dot(worldInvInertia1 * r1CrossN) + r2CrossN.Dot(worldInvInertia2 * r2CrossN);

        if (denom < 1e-8f)
            continue; 

        float jNormal = -(1.0f + e) * relVelAlongNormal / denom;
        FVector3 impulseN = collision.normal * jNormal;

        if (rb1)
            ApplyImpulse(*rb1, transform1, -impulseN, r1);
        if (rb2)
            ApplyImpulse(*rb2, transform2, impulseN, r2);

        FVector3 v1Post = rb1 ? (rb1->linearVelocity + rb1->angularVelocity.Cross(r1)) : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 v2Post = rb2 ? (rb2->linearVelocity + rb2->angularVelocity.Cross(r2)) : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 newRelVel = v2Post - v1Post;

        float normalComponent = newRelVel.Dot(collision.normal);
        FVector3 tangentialVel = newRelVel - (collision.normal * normalComponent);
        float tangentialLenSq = tangentialVel.LengthSquared();

        if (tangentialLenSq > 1e-8f)
        {
            FVector3 tangent = tangentialVel.Normalize();

            FVector3 r1CrossT = r1.Cross(tangent);
            FVector3 r2CrossT = r2.Cross(tangent);

            float denomT = (rb1 ? rb1->inverseMass : 0.0f) + (rb2 ? rb2->inverseMass : 0.0f) +
                           (rb1 ? r1CrossT.Dot(worldInvInertia1 * r1CrossT) : 0.0f) +
                           (rb2 ? r2CrossT.Dot(worldInvInertia2 * r2CrossT) : 0.0f);

            if (denomT > 1e-8f)
            {
                float jTangent = -newRelVel.Dot(tangent) / denomT;

                float mu = std::sqrt(collider1.friction * collider2.friction);
                jTangent = MathUtil::Clamp(jTangent, -mu * jNormal, mu * jNormal);

                FVector3 impulseT = tangent * jTangent;

                if (rb1)
                    ApplyImpulse(*rb1, transform1, -impulseT, r1);
                if (rb2)
                    ApplyImpulse(*rb2, transform2, impulseT, r2);
            }
        }

        float totalInvMass = (rb1 ? rb1->inverseMass : 0.0f) + (rb2 ? rb2->inverseMass : 0.0f);
        if (totalInvMass > 0.0f)
        {
            float penetration = std::max(collision.penetration - slop, 0.0f);
            FVector3 correction = collision.normal * (penetration / totalInvMass) * percent;
            if (rb1)
                transform1.position -= correction * rb1->inverseMass;
            if (rb2)
                transform2.position += correction * rb2->inverseMass;
        }
    }
}



void CollisionSystem::ApplyImpulse(RigidBodyComponent &body, const TransformComponent& transform, const FVector3 &impulse, const FVector3 &r)
{
    if (body.inverseMass <= 0.0f)
        return; 

    body.linearVelocity += impulse * body.inverseMass;

    const Matrix3 rotationMatrix = transform.rotation.GetRotationMatrix3();
    const Matrix3 worldInvInertia = rotationMatrix * body.localInverseInertiaTensor * rotationMatrix.Transpose();
    const FVector3 angularImpulse = worldInvInertia * r.Cross(impulse);
    body.angularVelocity += angularImpulse;
}

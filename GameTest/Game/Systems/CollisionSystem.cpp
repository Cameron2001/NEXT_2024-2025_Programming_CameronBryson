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

CollisionSystem::CollisionSystem(Registry *registry, EventManager *eventManager)
    : m_registry(registry), m_eventManager(eventManager), m_boxView(registry), m_sphereView(registry),
      m_dyanmicBoxView(registry), m_dynamicSphereView(registry), m_potentialCollisions(), m_threadCollisions(),
      m_collisions()
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
    const FVector3 minPoint(-100.0f, -100.0f, -100.0f);
    const FVector3 maxPoint(100.0f, 100.0f, 100.0f);
    const BoundingBox3D sceneBounds(minPoint, maxPoint);
    m_octree = std::make_unique<Octree>(sceneBounds);
    m_boxView.Update();
    m_sphereView.Update();
    m_boxView.ForEach([this](Entity entity, TransformComponent &transform, BoxBoundsComponent &boxBounds,
                             ColliderComponent &collider) { m_octree->Insert(boxBounds, transform, entity,collider.isDynamic); });

    // Insert sphere colliders into the octree
    m_sphereView.ForEach([this](Entity entity, TransformComponent &transform, SphereBoundsComponent &sphereBounds,
                                ColliderComponent &collider) { m_octree->Insert(sphereBounds, transform, entity,collider.isDynamic); });
}

void CollisionSystem::UpdateOctree()
{
    m_octree->ClearDynamicColliders();
    m_dyanmicBoxView.Update();
    m_dynamicSphereView.Update();
    m_dyanmicBoxView.ForEach(
        [this](Entity entity, TransformComponent &transform, BoxBoundsComponent &boxBounds,
               ColliderComponent &collider, RigidBodyComponent&) { m_octree->Insert(boxBounds, transform, entity, collider.isDynamic); });
    m_dynamicSphereView.ForEach(
        [this](Entity entity, TransformComponent &transform, SphereBoundsComponent &sphereBounds,
               ColliderComponent &collider, RigidBodyComponent &) { m_octree->Insert(sphereBounds, transform, entity, collider.isDynamic); });
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

    m_threadCollisions.local().emplace_back(collision);
    // m_collisions.emplace_back(collision);

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
        m_threadCollisions.local().emplace_back(ID1, ID2, contactPoint, collisionNormal, penetration);
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

    const float radius =
        sphereBounds.radius * (sphereTransform.Scale.x + sphereTransform.Scale.y + sphereTransform.Scale.z) / 3.0f;

    const FVector3 scaledBoxExtents = boxBounds.extents * boxTransform.Scale;

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
    float closestPointLocalX = (std::max)(-scaledBoxExtents.x, (std::min)(deltaLocalX, scaledBoxExtents.x));
    float closestPointLocalY = (std::max)(-scaledBoxExtents.y, (std::min)(deltaLocalY, scaledBoxExtents.y));
    float closestPointLocalZ = (std::max)(-scaledBoxExtents.z, (std::min)(deltaLocalZ, scaledBoxExtents.z));

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
            unsigned int entityID1 = collisionPair.first;
            unsigned int entityID2 = collisionPair.second;

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
    const float percent = 0.8f;                      
    const float slop = 0.001f;                       // Penetration allowance
    const float restitutionVelocityThreshold = 0.1f; 

    for (auto &collision : m_collisions)
    {
        unsigned int entityID1 = collision.ID1;
        unsigned int entityID2 = collision.ID2;

        bool hasRigidBody1 = m_registry->HasComponent<RigidBodyComponent>(entityID1);
        bool hasRigidBody2 = m_registry->HasComponent<RigidBodyComponent>(entityID2);

        // If neither has a RigidBody, no physics resolution needed
        // This should never happen because the collision would not be detected
        if (!hasRigidBody1 && !hasRigidBody2)
            continue;

        TransformComponent &transform1 = m_registry->GetComponent<TransformComponent>(entityID1);
        TransformComponent &transform2 = m_registry->GetComponent<TransformComponent>(entityID2);

        RigidBodyComponent *rb1 = hasRigidBody1 ? &m_registry->GetComponent<RigidBodyComponent>(entityID1) : nullptr;
        RigidBodyComponent *rb2 = hasRigidBody2 ? &m_registry->GetComponent<RigidBodyComponent>(entityID2) : nullptr;

        ColliderComponent &collider1 = m_registry->GetComponent<ColliderComponent>(entityID1);
        ColliderComponent &collider2 = m_registry->GetComponent<ColliderComponent>(entityID2);

        FVector3 v1 = rb1 ? rb1->linearVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 v2 = rb2 ? rb2->linearVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 w1 = rb1 ? rb1->angularVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 w2 = rb2 ? rb2->angularVelocity : FVector3(0.0f, 0.0f, 0.0f);

        FVector3 r1 = collision.contactPoint - transform1.Position;
        FVector3 r2 = collision.contactPoint - transform2.Position;

        Matrix3 rotationMatrix1 = transform1.Rotation.GetRotationMatrix3();
        Matrix3 rotationMatrix2 = transform2.Rotation.GetRotationMatrix3();

        Matrix3 worldInvInertia1 = (rb1)
                                       ? rotationMatrix1 * rb1->localInverseInertiaTensor * rotationMatrix1.Transpose()
                                       : Matrix3().SetZero();
        Matrix3 worldInvInertia2 = (rb2)
                                       ? rotationMatrix2 * rb2->localInverseInertiaTensor * rotationMatrix2.Transpose()
                                       : Matrix3().SetZero();

        FVector3 relativeVel = (v2 + w2.Cross(r2)) - (v1 + w1.Cross(r1));
        float relVelAlongNormal = relativeVel.Dot(collision.normal);


        if (relVelAlongNormal > 0.0f)
            continue;

        float elasticity1 = collider1.elasticity;
        float elasticity2 = collider2.elasticity;
        float combinedRestitution = 1.0f;
        if (rb1 && rb2)
            combinedRestitution = elasticity1 * elasticity2;
        else if (rb1)
            combinedRestitution = elasticity1;
        else if (rb2)
            combinedRestitution = elasticity2;

        if (fabs(relVelAlongNormal) < restitutionVelocityThreshold)
        {
            combinedRestitution = 0.0f;
        }

        combinedRestitution = (std::max)(0.0f, (std::min)(1.0f, combinedRestitution));

        float invMass1 = rb1 ? rb1->inverseMass : 0.0f;
        float invMass2 = rb2 ? rb2->inverseMass : 0.0f;

        FVector3 r1CrossN = r1.Cross(collision.normal);
        FVector3 r2CrossN = r2.Cross(collision.normal);

        float angTerm1 = rb1 ? r1CrossN.Dot(worldInvInertia1 * r1CrossN) : 0.0f;
        float angTerm2 = rb2 ? r2CrossN.Dot(worldInvInertia2 * r2CrossN) : 0.0f;

        float normalImpulseDenominator = invMass1 + invMass2 + angTerm1 + angTerm2;
        float normalImpulseMag = 0.0f;
        if (normalImpulseDenominator > 1e-8f)
        {
            normalImpulseMag = -(1.0f + combinedRestitution) * relVelAlongNormal;
            normalImpulseMag /= normalImpulseDenominator;
        }

        FVector3 normalImpulse = collision.normal * normalImpulseMag;

        if (rb1)
        {
            rb1->linearVelocity -= normalImpulse * invMass1;
            rb1->angularVelocity += worldInvInertia1 * r1CrossN * normalImpulseMag;
        }
        if (rb2)
        {
            rb2->linearVelocity += normalImpulse * invMass2;
            rb2->angularVelocity += worldInvInertia2 * r2CrossN * -normalImpulseMag;
        }

        FVector3 v1Post = rb1 ? rb1->linearVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 w1Post = rb1 ? rb1->angularVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 v2Post = rb2 ? rb2->linearVelocity : FVector3(0.0f, 0.0f, 0.0f);
        FVector3 w2Post = rb2 ? rb2->angularVelocity : FVector3(0.0f, 0.0f, 0.0f);

        FVector3 newRelativeVel = (v2Post + w2Post.Cross(r2)) - (v1Post + w1Post.Cross(r1));

        FVector3 tangent = newRelativeVel - collision.normal * newRelativeVel.Dot(collision.normal);
        float tangentLenSq = tangent.LengthSquared();
        FVector3 frictionImpulse(0.0f, 0.0f, 0.0f);

        if (tangentLenSq > 1e-8f)
        {
            tangent = tangent.Normalize();

            FVector3 r1CrossT = r1.Cross(tangent);
            FVector3 r2CrossT = r2.Cross(tangent);

            float tAngTerm1 = rb1 ? r1CrossT.Dot(worldInvInertia1 * r1CrossT) : 0.0f;
            float tAngTerm2 = rb2 ? r2CrossT.Dot(worldInvInertia2 * r2CrossT) : 0.0f;

            float frictionDenominator = invMass1 + invMass2 + tAngTerm1 + tAngTerm2;
            if (frictionDenominator > 1e-8f)
            {
                float relVelAlongTangent = newRelativeVel.Dot(tangent);

                float muStatic1 = collider1.staticFriction;
                float muStatic2 = collider2.staticFriction;
                float muDynamic1 = collider1.dynamicFriction;
                float muDynamic2 = collider2.dynamicFriction;

                float combinedStaticFriction =
                    (rb1 && rb2) ? sqrt(muStatic1 * muStatic2) : (rb1 ? muStatic1 : muStatic2);
                float combinedDynamicFriction =
                    (rb1 && rb2) ? sqrt(muDynamic1 * muDynamic2) : (rb1 ? muDynamic1 : muDynamic2);

                float frictionMag = -relVelAlongTangent / frictionDenominator;

                float normalImpulseMagAbs = fabs(normalImpulseMag);
                float maxStaticFriction = combinedStaticFriction * normalImpulseMagAbs;
                float maxDynamicFriction = combinedDynamicFriction * normalImpulseMagAbs;

                if (fabs(frictionMag) < maxStaticFriction)
                {
                    frictionImpulse = tangent * frictionMag;
                }
                else
                {
                    float clamped = (fabs(frictionMag) > maxDynamicFriction)
                                        ? (frictionMag < 0.0f ? -maxDynamicFriction : maxDynamicFriction)
                                        : frictionMag;

                    frictionImpulse = tangent * clamped;
                }
            }
        }

        if (rb1)
        {
            rb1->linearVelocity -= frictionImpulse * invMass1;
            rb1->angularVelocity -= worldInvInertia1 * r1.Cross(frictionImpulse);
        }
        if (rb2)
        {
            rb2->linearVelocity += frictionImpulse * invMass2;
            rb2->angularVelocity += worldInvInertia2 * r2.Cross(frictionImpulse);
        }

        float totalInvMass = invMass1 + invMass2;
        if (totalInvMass > 0.0f)
        {
            float penetrationDepth = collision.penetration;
            float penetrationToCorrect = (std::max)(penetrationDepth - slop, 0.0f);

            FVector3 correction = collision.normal * (percent * penetrationToCorrect / totalInvMass);

            if (rb1)
                transform1.Position -= correction * invMass1;
            if (rb2)
                transform2.Position += correction * invMass2;
        }
    }
}

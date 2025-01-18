#pragma once
#include <memory>
#include "Game/Core/Components.h"
#include "Game/Storage/Registry.h"
#include "Game/Core/Collision.h"
#include <Game/Math/Octree.h>
#include <vector>
#include <Game/Storage/IComponentArray.h>
#include <Game/Math/Matrix4.h>
#include <ppl.h>
#include "Game/Managers/EventManager.h"
class CollisionSystem
{
  public:
    CollisionSystem(Registry *registry, EventManager *eventManager);
    ~CollisionSystem() = default;
    void Init();
    void Update(float dt);
    void LateUpdate(float dt);
    void Shutdown();

  private:
    void BuildOctree();
    bool TestAxisOverlap(const FVector3 &axis, const BoxBoundsComponent &box1, const FVector3 &scale1,
                         const Matrix4 &rotation1, const BoxBoundsComponent &box2, const FVector3 &scale2,
                         const Matrix4 &rotation2, const FVector3 &translation, float &minimalPenetration,
                         FVector3 &collisionNormal) const;
    bool OOBvsOOB(Entity ID1, Entity ID2);
    bool SpherevsSphere(Entity ID1, Entity ID2);
    bool SpherevsOOB(Entity ID1, Entity ID2);
    bool CanCollide(const ColliderComponent &collider1, const ColliderComponent &collider2);

    void DetectCollisions();
    void ResolveCollisions();
    Registry *m_registry;
    EventManager *m_eventManager;
    std::unique_ptr<Octree> m_octree;
    // We should decide if we want to defer collision resolution. Or resolve as they are found.
    std::vector<std::pair<unsigned int, unsigned int>> m_potentialCollisions;
    concurrency::combinable<std::vector<Collision>> m_threadCollisions;
    std::vector<Collision> m_collisions;
    View<TransformComponent, BoxBoundsComponent, ColliderComponent> m_boxView;
    View<TransformComponent, SphereBoundsComponent, ColliderComponent> m_sphereView;
};

// Read
//  Transform
//  Collider
//  RigidBody
//  BoxBounds
//  SphereBounds

// Write
//  CollisionManager

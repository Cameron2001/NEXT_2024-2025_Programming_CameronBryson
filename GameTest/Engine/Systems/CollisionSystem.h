#pragma once
#include <memory>
#include "Engine/Core/Components.h"
#include "Engine/Storage/Registry.h"
#include "Engine/Core/Collision.h"
#include <Engine/Math/Octree.h>
#include <vector>
#include <Engine/Storage/IComponentArray.h>
class CollisionSystem
{
  public:
    CollisionSystem(Registry *registry);
    ~CollisionSystem() = default;
    void Init();
    void Update(float dt);
    void LateUpdate(float dt);
    void Shutdown();

  private:
    void BuildOctree();
    bool OOBvsOOB(Entity ID1, Entity ID2);
    bool SpherevsSphere(Entity ID1, Entity ID2);
    bool SpherevsOOB(Entity ID1, Entity ID2);
    bool CanCollide(const ColliderComponent& collider1, const ColliderComponent& collider2);

    void DetectCollisions();
    void ResolveCollisions();
    Registry *m_registry;
    std::unique_ptr<Octree> m_octree;
    //We should decide if we want to defer collision resolution. Or resolve as they are found.
    std::vector<Collision> m_collisions;
};

// Read
//  Transform
//  Collider
//  RigidBody
//  BoxBounds
//  SphereBounds

// Write
//  CollisionManager

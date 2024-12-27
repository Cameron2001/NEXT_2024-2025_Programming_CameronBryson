#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Engine/Storage/Registry.h"
#include "Engine/Core/Collision.h"
#include <unordered_map>
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
    bool OOBvsOOB(Entity ID1, Entity ID2);
    bool SpherevsSphere(Entity ID1, Entity ID2);
    bool SpherevsOOB(Entity ID1, Entity ID2);
    // std::unordered_map<std::pair<Entity, Entity>, Collision> m_collisions;
    //  Raw pointers are fine. This will be destroyed before the dependencys
    Registry *m_registry;
};

// Read
//  Transform
//  Collider
//  RigidBody
//  BoxBounds
//  SphereBounds

// Write
//  CollisionManager

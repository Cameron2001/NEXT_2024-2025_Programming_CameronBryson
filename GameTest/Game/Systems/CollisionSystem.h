#pragma once
#include <memory>
#include "Game/Core/Components.h"
#include "Game/Storage/Registry.h"
#include "Game/Core/Collision.h"
#include <Game/Math/Octree.h>
#include <vector>
#include <Game/Math/Matrix4.h>
#include <ppl.h>
#include "Game/Managers/EventManager.h"
class CollisionSystem
{
  public:
    CollisionSystem(Registry *registry, EventManager *eventManager);
    ~CollisionSystem() = default;
    void Init();
    void LateInit();
    void Update(float dt);
    void LateUpdate(float dt);
    void Shutdown();

  private:
    void BuildOctree();
    void UpdateOctree();
    static bool TestAxisOverlap(const FVector3 &axis, const BoxBoundsComponent &box1, const FVector3 &scale1,
                                const Matrix4 &rotation1, const BoxBoundsComponent &box2, const FVector3 &scale2,
                                const Matrix4 &rotation2, const FVector3 &translation, float &minimalPenetration,
                                FVector3 &collisionNormal);
    bool OOBvsOOB(Entity ID1, Entity ID2);
    bool SpherevsSphere(Entity ID1, Entity ID2);
    bool SpherevsOOB(Entity ID1, Entity ID2);
    bool CanCollide(const ColliderComponent &collider1, const ColliderComponent &collider2);

    void DetectCollisions();
    void ResolveCollisions(float dt);
    void ApplyImpulse(RigidBodyComponent &body, const TransformComponent &transform, const FVector3 &impulse,
                      const FVector3 &r);
    Registry *m_registry;
    EventManager *m_eventManager;
    std::unique_ptr<Octree> m_octree;
    std::vector<std::pair<unsigned int, unsigned int>> m_potentialCollisions;
    concurrency::combinable<std::vector<Collision>> m_threadCollisions;
    std::vector<Collision> m_collisions;
    View<TransformComponent, BoxBoundsComponent, ColliderComponent> m_boxView;
    View<TransformComponent, BoxBoundsComponent, ColliderComponent, RigidBodyComponent> m_dyanmicBoxView;
    View<TransformComponent, SphereBoundsComponent, ColliderComponent> m_sphereView;
    View<TransformComponent, SphereBoundsComponent, ColliderComponent, RigidBodyComponent> m_dynamicSphereView;
};
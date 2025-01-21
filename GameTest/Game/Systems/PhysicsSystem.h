#pragma once
#include <memory>
#include "Game/Storage/ComponentArray.h"
#include "Game/Core/Components.h"
#include <Game/Storage/Registry.h>

class PhysicsSystem
{
  public:
    PhysicsSystem(Registry *registry);
    ~PhysicsSystem() = default;
    void Init();
    void Update(float dt);
    void Shutdown();

  private:
    static void ComputeBoxInverseInertiaTensor(const TransformComponent &transform, const BoxBoundsComponent &boxBounds,
                                               RigidBodyComponent &rigidBody);
    static void ComputeSphereInverseInertiaTensor(const TransformComponent &tranform,
                                                  const SphereBoundsComponent &sphereBounds, RigidBodyComponent &rigidBody);
    Registry *m_registry;
    View<TransformComponent, RigidBodyComponent> m_view;
    View<TransformComponent, BoxBoundsComponent, RigidBodyComponent> m_boxView;
    View<TransformComponent, SphereBoundsComponent, RigidBodyComponent> m_sphereView;
};

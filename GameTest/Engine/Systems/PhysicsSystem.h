#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include <Engine/Storage/Registry.h>

class PhysicsSystem
{
  public:
    PhysicsSystem(Registry *registry);
    ~PhysicsSystem() = default;
    void Init();
    void Update(float dt);
    void Shutdown();

  private:
    void ComputeBoxInverseInertiaTensor(const TransformComponent &transform, const BoxBoundsComponent &boxBounds,
                                        RigidBodyComponent &rigidBody);
    void ComputeSphereInverseInertiaTensor(const TransformComponent &tranform,
                                           const SphereBoundsComponent &sphereBounds, RigidBodyComponent &rigidBody);
    Registry *m_registry;
    View<TransformComponent, RigidBodyComponent> m_view;
    View<TransformComponent, BoxBoundsComponent, RigidBodyComponent> m_boxView;
    View<TransformComponent, SphereBoundsComponent, RigidBodyComponent> m_sphereView;
};

// Read:
// transform
// rigidbody

// Write:
// transform
// rigidbody

#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Engine/Managers/CollisionManager.h"

class CollisionDetectionSystem
{
public:
    CollisionDetectionSystem();
    ~CollisionDetectionSystem() = default;
    void Init();
    void Update(float dt);
    void Shutdown();

private:
    bool OOBvsOOB(Entity ID1, Entity ID2);
    bool SpherevsSphere(Entity ID1, Entity ID2);
    bool SpherevsOOB(Entity ID1, Entity ID2);
};

//Read
// Transform
// Collider
// RigidBody
// BoxBounds
// SphereBounds

//Write
// CollisionManager
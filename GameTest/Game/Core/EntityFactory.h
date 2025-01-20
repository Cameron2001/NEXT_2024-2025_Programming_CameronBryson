#pragma once
#include "Game/Storage/Registry.h"
#include "Game/Math/Vector3.h"
class EntityFactory
{
  public:
    EntityFactory(Registry *registry);

    Entity CreateGolfBallOne(const FVector3 &position);
    Entity CreateGolfBallTwo(const FVector3 &position);
    Entity CreateDynamicSphere(const FVector3 &position, float radius);
    Entity CreateStaticBox(const FVector3 &position, const FVector3 &extents, const FVector3 &rotation,
                           const FVector3 &color);
    Entity CreateGrassBox(const FVector3 &position, const FVector3 &scale, const FVector3 &color);
    Entity CreateFlag(const FVector3 &position);
    Entity CreateArrow(Entity followTarget);
    Entity CreateBorder(const FVector3 &position, const FVector3 &scale, const FVector3 &color);
    Entity CreateInvisibleBoxCollider(const FVector3 &position, const FVector3 &extents, const float elasticity,
                                      const float staticFriction, const float dynamicFriction);
    Entity CreateBouncyBox(const FVector3 &position, const FVector3 &extents);
    Entity CreateBouncySphere(const FVector3 &position, float radius);
    Entity CreateSandBox(const FVector3 &position, const FVector3 &extents);
    Entity CreateIceBox(const FVector3 &position, const FVector3 &extents);



  private:
    Registry *m_registry;
};

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
    Entity CreateStaticSphere(const FVector3 &position, float radius);
    Entity CreateDynamicBox(const FVector3 &position, const FVector3 &extents);
    Entity CreateStaticBox(const FVector3 &position, const FVector3 &extents);
    Entity CreateGrassBox(const FVector3 &position, const FVector3 &scale, const FVector3 &color);
    Entity CreateFlag(const FVector3 &position);
    Entity CreateArrow(Entity followTarget);


  private:
    Registry *m_registry;
};

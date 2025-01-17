#pragma once
#include "Game/Storage/Registry.h"
#include "Game/Math/Vector3.h"
class EntityFactory
{
  public:
    EntityFactory(Registry *registry);

    Entity CreatePlayer(const FVector3 &position);
    Entity CreateDynamicSphere(const FVector3 &position, float radius);
    Entity CreateStaticSphere(const FVector3 &position, float radius);
    Entity CreateDynamicBox(const FVector3 &position, const FVector3 &extents);
    Entity CreateStaticBox(const FVector3 &position, const FVector3 &extents);
    Entity CreateGolfBall(const FVector3 &position);

  private:
    Registry *m_registry;
};

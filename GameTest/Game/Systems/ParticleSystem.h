#pragma once
#include "Game/Storage/Registry.h"
#include <Game/Math/Vector2.h>
#include "Game/Managers/EventManager.h"
class ParticleSystem
{
  public:
    explicit ParticleSystem(Registry *registry, EventManager *eventManager);
    void Update(float deltaTime);
    void Render();
    void EmitParticles(FVector2 position, int count, FVector3 color);

  private:
    Registry *m_registry;
    EventManager *m_eventManager;

  private:
    float GenerateRandomLifeTime();
    FVector2 GenerateRandomAcceleration();
    View<ParticleComponent> m_view;
};

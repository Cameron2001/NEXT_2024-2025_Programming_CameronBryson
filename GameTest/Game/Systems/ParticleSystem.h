#pragma once
#include "Game/Storage/Registry.h"
#include <Game/Math/Vector2.h>
class ParticleSystem
{
  public:
    explicit ParticleSystem(Registry *registry);
    void Update(float deltaTime);
    void Render();
    void EmitParticles(const FVector2 &position, int count);

  private:
    Registry *m_registry;

  private:
    float GenerateRandomLifeTime();
    FVector2 GenerateRandomAcceleration();
    View<ParticleComponent> m_view;
};

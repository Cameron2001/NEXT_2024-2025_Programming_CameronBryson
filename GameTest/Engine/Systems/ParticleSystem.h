#pragma once
#include "Engine/Storage/Registry.h"
class ParticleSystem
{
  public:
    ParticleSystem(Registry *registry);
    void Update(float deltaTime);
    void Render();
    void EmitParticles(const FVector3 &position, int count);

  private:
    Registry *m_registry;

  private:
    float GenerateRandomLifeTime();
    FVector3 GenerateRandomAcceleration();
};

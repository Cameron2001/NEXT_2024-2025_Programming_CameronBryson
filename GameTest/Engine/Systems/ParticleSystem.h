#pragma once
#include "Engine/Storage/Registry.h"
class ParticleSystem
{
  public:
    ParticleSystem(Registry *registry);
    void Update(float deltaTime);
    void Render() const;
    void EmitParticles(const FVector2 &position, int count);

  private:
    Registry *m_registry;

  private:
    float GenerateRandomLifeTime();
    FVector2 GenerateRandomAcceleration();
};

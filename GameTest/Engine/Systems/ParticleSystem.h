#pragma once
#include "Engine/Storage/Registry.h"
#include <Engine/Math/Vector2.h>
class ParticleSystem
{
  public:
    explicit ParticleSystem(Registry *registry);
    void Update(float deltaTime);
    void Render() const;
    void EmitParticles(const FVector2 &position, int count);

  private:
    Registry *m_registry;

  private:
    float GenerateRandomLifeTime();
    FVector2 GenerateRandomAcceleration();
};

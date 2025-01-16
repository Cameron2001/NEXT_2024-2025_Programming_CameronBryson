#include "stdafx.h"
#include "Engine/Core/Components.h"
#include "ParticleSystem.h"
#include "Engine/Graphics/Renderer.h"
#include "App/AppSettings.h"
#include <concurrent_vector.h>
#include <tuple>
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/IComponentArray.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>

ParticleSystem::ParticleSystem(Registry *registry) : m_registry(registry), m_view(registry)
{
}

void ParticleSystem::Update(float deltaTime)
{
    m_view.Update();

    // We dont want to make new one every frame, store this in class
    // We should also figure out what is better, combinable vector or concurrent vector
    Concurrency::concurrent_vector<Entity> entitiesToDestroy;

    m_view.ParallelForEach([&](Entity entity, ParticleComponent &particle) {
        particle.position += particle.linearVelocity * deltaTime;
        particle.rotation += particle.angularVelocity * deltaTime;
        particle.age += deltaTime;

        // Check if particle should be destroyed
        if (particle.age > particle.lifetime)
        {
            entitiesToDestroy.push_back(entity);
        }
    });

    for (const auto &entity : entitiesToDestroy)
    {
        m_registry->DestroyEntity(entity);
    }
}

void ParticleSystem::Render()
{
    m_view.Update();
    m_view.ForEach([&](Entity entity, ParticleComponent &particle) {
        Renderer::DrawParticle(particle.position, 0.02f, particle.rotation, FVector3(1.0f, 0.0f, 0.0f));
    });
}

void ParticleSystem::EmitParticles(const FVector2 &position, const int count)
{
    for (int i = 0; i < count; ++i)
    {
        const auto entity = m_registry->CreateEntity();
        m_registry->AddComponent<ParticleComponent>(entity, position, 0, GenerateRandomAcceleration(),
                                                    FRAND_RANGE(1.0f, 25.0f), GenerateRandomLifeTime());
    }
}

float ParticleSystem::GenerateRandomLifeTime()
{
    return FRAND_RANGE(0.2f, 0.5f);
}

FVector2 ParticleSystem::GenerateRandomAcceleration()
{
    return FVector2(FRAND_RANGE(-0.2f, 0.2f), FRAND_RANGE(-0.2f, 0.2f));
}

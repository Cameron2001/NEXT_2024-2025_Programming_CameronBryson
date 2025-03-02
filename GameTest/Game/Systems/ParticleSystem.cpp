#include "stdafx.h"
#include "Game/Core/Components.h"
#include "ParticleSystem.h"
#include "Game/Graphics/Renderer.h"
#include "App/AppSettings.h"
#include <concurrent_vector.h>
#include <tuple>
#include <Game/Math/Vector2.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/IComponentArray.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>

ParticleSystem::ParticleSystem(Registry *registry, EventManager *eventManager) : m_registry(registry), m_eventManager(eventManager), m_view(registry)
{
}

void ParticleSystem::Update(const float deltaTime)
{
    m_view.Update();

    // We dont want to make new one every frame, store this in class
    // We should also figure out what is better, combinable vector or concurrent vector
    Concurrency::concurrent_vector<Entity> entitiesToDestroy;

    m_view.ParallelForEach([&](const Entity entity, ParticleComponent &particle) {
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
    m_view.ForEach([&](Entity entity, const ParticleComponent &particle) {
        Renderer::DrawParticle(particle.position, 0.02f, particle.rotation, particle.color);
    });
}

void ParticleSystem::EmitParticles(FVector2 position, int count, FVector3 color)
{


    for (int i = 0; i < count; ++i)
    {
        const auto entity = m_registry->CreateEntity();
        m_registry->AddComponent<ParticleComponent>(entity, position, color, 0, GenerateRandomAcceleration(),
                                                    FRAND_RANGE(1.0f, 25.0f), GenerateRandomLifeTime());
    }
}

float ParticleSystem::GenerateRandomLifeTime()
{
    return FRAND_RANGE(0.6f, 1.5f);
}

FVector2 ParticleSystem::GenerateRandomAcceleration()
{
    return FVector2(FRAND_RANGE(-0.5f, 0.5f), FRAND_RANGE(-0.5f, 0.5f));
}

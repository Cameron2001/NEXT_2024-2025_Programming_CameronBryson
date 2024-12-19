#include "stdafx.h"
#include "Engine/Core/Components.h"
#include "ParticleSystem.h"
#include "Engine/Graphics/Renderer2D.h"
#include "App/AppSettings.h"
#include <concurrent_vector.h>

ParticleSystem::ParticleSystem(Registry *registry) : m_registry(registry)
{
}

void ParticleSystem::Update(float deltaTime)
{
    auto view = m_registry->CreateView<ParticleComponent>();

    Concurrency::concurrent_vector<Entity> entitiesToDestroy;

    view.ParallelForEach([&](const auto &entityTuple) {
        Entity entity = std::get<0>(entityTuple);
        auto &particle = std::get<1>(entityTuple);

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
    auto view = m_registry->CreateView<ParticleComponent>();
    for (auto &&entity : view)
    {
        auto &particle = std::get<1>(entity);
        Renderer2D::DrawParticle(particle.position, 0.02f, particle.rotation, FVector3(1.0f, 0.0f, 0.0f));
    }
}

void ParticleSystem::EmitParticles(const FVector2 &position, int count)
{
    for (int i = 0; i < count; ++i)
    {
        auto entity = m_registry->CreateEntity();
        m_registry->AddComponent<ParticleComponent>(entity, position, 0, GenerateRandomAcceleration(),
                                                    FRAND_RANGE(1.0f, 25.0f), GenerateRandomLifeTime());
    }
    // particles need transform rigidbody and particle components
    // Also have to figure out how we are rendering it. Do we treat it as a model?
    // I think i should probably just draw lines. since we dont need triangles or proper winding order
    // Dont need to interact with other objects
}

float ParticleSystem::GenerateRandomLifeTime()
{
    return FRAND_RANGE(0.2f, 0.5f);
}

FVector2 ParticleSystem::GenerateRandomAcceleration()
{
    return FVector2(FRAND_RANGE(-0.2f, 0.2f), FRAND_RANGE(-0.2f, 0.2f));
}

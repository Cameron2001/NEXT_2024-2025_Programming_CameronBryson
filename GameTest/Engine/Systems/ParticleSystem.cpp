#include "stdafx.h"
#include "Engine/Core/Components.h"
#include "ParticleSystem.h"
#include "Engine/Graphics/Renderer2D.h"
#include "App/AppSettings.h"

ParticleSystem::ParticleSystem(Registry *registry) : m_registry(registry)
{
}

void ParticleSystem::Update(float deltaTime)
{ // Update particles
    auto view = m_registry->CreateView<ParticleComponent>();
    for (auto &&entity : view)
    {
        auto &particle = std::get<1>(entity);
        particle.position += particle.linearVelocity * deltaTime;
        particle.rotation += particle.angularVelocity * deltaTime;
        particle.age += deltaTime;
        if (particle.age > particle.lifetime)
        {
            m_registry->DestroyEntity(std::get<0>(entity));
        }
    }
}

void ParticleSystem::Render()
{
    auto view = m_registry->CreateView<ParticleComponent>();
    for (auto &entity : view)
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
    // I think i should probably just draw lines. since we dont need faces or proper winding order
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

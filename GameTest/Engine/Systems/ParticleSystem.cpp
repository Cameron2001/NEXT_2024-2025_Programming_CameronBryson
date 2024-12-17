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
        particle.age += deltaTime;
        if (particle.age > particle.lifetime)
        {
            m_registry->DestroyEntity(std::get<0>(entity));
        }
    }
}

void ParticleSystem::Render()
{
    auto view = m_registry->CreateView<TransformComponent, ParticleComponent>();
    for (auto &entity : view)
    {
        auto &transform = std::get<1>(entity);
        auto &particle = std::get<2>(entity);
        Renderer2D::DrawCross(transform.Position, 0.1f, FVector3(1.0f, 0.0f, 0.0f));
        // Draw particle
    }
}

void ParticleSystem::EmitParticles(const FVector3 &position, int count)
{
    for (int i = 0; i < count; ++i)
    {
        auto entity = m_registry->CreateEntity();
        m_registry->AddComponent<TransformComponent>(entity, position, FVector3(0.0f, 0.0f, 0.0f),
                                                     FVector3(1.0f, 1.0f, 1.0f));
        m_registry->AddComponent<ParticleComponent>(entity, GenerateRandomLifeTime());
        m_registry->AddComponent<RigidBodyComponent>(entity, 0, 0, GenerateRandomAcceleration(),
                                                     GenerateRandomAcceleration());
    }
    // particles need transform rigidbody and particle components
    // Also have to figure out how we are rendering it. Do we treat it as a model?
    // I think i should probably just draw lines. since we dont need faces or proper winding order
    // Dont need to interact with other objects
}

float ParticleSystem::GenerateRandomLifeTime()
{
    return FRAND_RANGE(10.0f, 300.0f);
}

FVector3 ParticleSystem::GenerateRandomAcceleration()
{
    return FVector3(FRAND_RANGE(-0.00001f, 0.00001f), FRAND_RANGE(-0.00001f, 0.00001f),
                    FRAND_RANGE(-0.00001f, 0.00001f));
}

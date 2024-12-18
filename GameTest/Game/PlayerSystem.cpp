#include "stdafx.h"
#include "PlayerSystem.h"
#include <App/app.h>

PlayerSystem::PlayerSystem(Registry *registry)
{
    m_registry = registry;
}

void PlayerSystem::Init()
{
}

void PlayerSystem::Update(float dt)
{
    auto view = m_registry->CreateView<PlayerComponent, RigidBodyComponent>();

    // Get input states
    bool UP = App::IsKeyPressed('W') || App::GetController().GetLeftThumbStickY() > 0.5f;
    bool LEFT = App::IsKeyPressed('A') || App::GetController().GetLeftThumbStickX() < -0.5f;
    bool DOWN = App::IsKeyPressed('S') || App::GetController().GetLeftThumbStickY() < -0.5f;
    bool RIGHT = App::IsKeyPressed('D') || App::GetController().GetLeftThumbStickX() > 0.5f;
    bool FORWARD = App::IsKeyPressed('Q');
    bool BACKWARD = App::IsKeyPressed('E');
    bool R = App::IsKeyPressed('R');
    bool F = App::IsKeyPressed('F');
    bool V = App::IsKeyPressed('V');

    constexpr float rotationSpeed = 1000.0f;

    // Update each player entity
    for (auto &&entity : view)
    {
        auto &player = std::get<1>(entity);
        auto &rigidbody = std::get<2>(entity);

        // Movement
        if (UP)
            rigidbody.linearAcceleration += FVector3{0, 1, 0} * player.moveSpeed * dt;
        if (DOWN)
            rigidbody.linearAcceleration -= FVector3{0, 1, 0} * player.moveSpeed * dt;
        if (LEFT)
            rigidbody.linearAcceleration -= FVector3{1, 0, 0} * player.moveSpeed * dt;
        if (RIGHT)
            rigidbody.linearAcceleration += FVector3{1, 0, 0} * player.moveSpeed * dt;
        if (FORWARD)
            rigidbody.linearAcceleration -= FVector3{0, 0, 1} * player.moveSpeed * dt;
        if (BACKWARD)
            rigidbody.linearAcceleration += FVector3{0, 0, 1} * player.moveSpeed * dt;

        // Rotation
        if (R)
            rigidbody.angularAcceleration += FVector3{0, 0, 1} * rotationSpeed * dt;
        if (F)
            rigidbody.angularAcceleration += FVector3{0, 1, 0} * rotationSpeed * dt;
        if (V)
            rigidbody.angularAcceleration += FVector3{1, 0, 0} * rotationSpeed * dt;
    }
}

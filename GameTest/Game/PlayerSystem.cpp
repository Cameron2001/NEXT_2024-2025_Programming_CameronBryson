#include "stdafx.h"
#include "PlayerSystem.h"
#include <App/app.h>

PlayerSystem::PlayerSystem(Registry* registry)
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
    bool W = App::IsKeyPressed('W');
    bool A = App::IsKeyPressed('A');
    bool S = App::IsKeyPressed('S');
    bool D = App::IsKeyPressed('D');
    bool Q = App::IsKeyPressed('Q');
    bool E = App::IsKeyPressed('E');
    bool R = App::IsKeyPressed('R');
    bool F = App::IsKeyPressed('F');
    bool V = App::IsKeyPressed('V');

    constexpr float rotationSpeed = 0.000001f;

    // Update each player entity
    for (auto it = view.begin(); it != view.end(); ++it)
    {
        auto &player = std::get<0>(*it);
        auto &rigidbody = std::get<1>(*it);

        // Movement
        if (W)
            rigidbody.linearAcceleration += FVector3{0, 1, 0} * player.moveSpeed * dt;
        if (S)
            rigidbody.linearAcceleration -= FVector3{0, 1, 0} * player.moveSpeed * dt;
        if (A)
            rigidbody.linearAcceleration -= FVector3{1, 0, 0} * player.moveSpeed * dt;
        if (D)
            rigidbody.linearAcceleration += FVector3{1, 0, 0} * player.moveSpeed * dt;
        if (Q)
            rigidbody.linearAcceleration -= FVector3{0, 0, 1} * player.moveSpeed * dt;
        if (E)
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
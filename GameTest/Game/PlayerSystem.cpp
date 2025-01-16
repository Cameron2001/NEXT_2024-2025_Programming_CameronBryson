#include "stdafx.h"
#include "GameComponents.h"
#include "PlayerSystem.h"
#include <App/app.h>
#include <Engine/Core/Components.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>

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

    constexpr float rotationSpeed = 1.0f;

    // Process entities in parallel with updated lambda signature
    view.ParallelForEach([&](Entity entity, PlayerComponent &player, RigidBodyComponent &rigidbody) {
        // Movement
        if (UP)
            rigidbody.force += FVector3{0.0f, 1.0f, 0.0f} * player.moveSpeed;
        if (DOWN)
            rigidbody.force -= FVector3{0.0f, 1.0f, 0.0f} * player.moveSpeed;
        if (LEFT)
            rigidbody.force -= FVector3{1.0f, 0.0f, 0.0f} * player.moveSpeed;
        if (RIGHT)
            rigidbody.force += FVector3{1.0f, 0.0f, 0.0f} * player.moveSpeed;
        if (FORWARD)
            rigidbody.force -= FVector3{0.0f, 0.0f, 1.0f} * player.moveSpeed;
        if (BACKWARD)
            rigidbody.force += FVector3{0.0f, 0.0f, 1.0f} * player.moveSpeed;

        // Rotation
        if (R)
            rigidbody.torque += FVector3{0.0f, 0.0f, 1.0f} * rotationSpeed;
        if (F)
            rigidbody.torque += FVector3{0.0f, 1.0f, 0.0f} * rotationSpeed;
        if (V)
            rigidbody.torque += FVector3{1.0f, 0.0f, 0.0f} * rotationSpeed;
    });
}

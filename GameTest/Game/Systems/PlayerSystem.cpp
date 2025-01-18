#include "stdafx.h"
#include "PlayerSystem.h"
#include <App/app.h>
#include <Game/Core/Components.h>
#include <Game/Core/Entity.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>

PlayerSystem::PlayerSystem(Registry *registry, EventManager *eventManager, PlayerManager* playerManager)
{
    m_registry = registry;
    m_eventManager = eventManager;
    m_playerManager = playerManager;
}

void PlayerSystem::Init()
{
    
}

void PlayerSystem::Update(float dt)
{
    auto view = m_registry->CreateView<PlayerComponent, RigidBodyComponent, TransformComponent>();

    // Get input states
    bool UP = App::IsKeyPressed('Q');
    bool LEFT = App::IsKeyPressed('A');
    bool DOWN = App::IsKeyPressed('E');
    bool RIGHT = App::IsKeyPressed('D');
    bool FORWARD = App::IsKeyPressed('W');
    bool BACKWARD = App::IsKeyPressed('S');
    bool R = App::IsKeyPressed('R');
    bool F = App::IsKeyPressed('F');
    bool V = App::IsKeyPressed('V');
    bool B = App::IsKeyPressed('B');


    constexpr float rotationSpeed = 1.0f;
     

    // Process entities in parallel with updated lambda signature
    view.ForEach([&](Entity entity, PlayerComponent &player, RigidBodyComponent &rigidbody, TransformComponent &transform) {
        FVector3 playerDirection = transform.Rotation.GetRotationMatrix3() * FVector3{0.0f, 0.0f, 1.0f};
        // Movement
        if (UP)
            rigidbody.force += FVector3{0.0f, 1.0f, 0.0f};
        if (DOWN)
            rigidbody.force -= FVector3{0.0f, 1.0f, 0.0f};
        if (LEFT)
            rigidbody.force -= FVector3{1.0f, 0.0f, 0.0f};
        if (RIGHT)
            rigidbody.force += FVector3{1.0f, 0.0f, 0.0f};
        if (FORWARD)
            rigidbody.force -= FVector3{0.0f, 0.0f, 1.0f};
        if (BACKWARD)
            rigidbody.force += FVector3{0.0f, 0.0f, 1.0f};

        // Rotation
        if (R)
            rigidbody.torque += FVector3{0.0f, 0.0f, 1.0f} * rotationSpeed;
        if (F)
            rigidbody.torque += FVector3{0.0f, 1.0f, 0.0f} * rotationSpeed;
        if (V)
            rigidbody.torque += FVector3{1.0f, 0.0f, 0.0f} * rotationSpeed;
        if (B)
        {
            rigidbody.torque = FVector3{0.0f, 0.0f, 0.0f};
            rigidbody.angularVelocity = FVector3{0.0f, 0.0f, 0.0f};
            rigidbody.force = FVector3{0.0f, 0.0f, 0.0f};
            rigidbody.linearVelocity = FVector3{0.0f, 0.0f, 0.0f};
            transform.Position = player.spawnPoint;
        }
        if (transform.Position.y <= -5.0f)
        {
            rigidbody.torque = FVector3{0.0f, 0.0f, 0.0f};
            rigidbody.angularVelocity = FVector3{0.0f, 0.0f, 0.0f};
            rigidbody.force = FVector3{0.0f, 0.0f, 0.0f};
            rigidbody.linearVelocity = FVector3{0.0f, 0.0f, 0.0f};
            transform.Position = player.spawnPoint;
        }
        
    });
}

void PlayerSystem::OnCollision(unsigned int ID1, unsigned int ID2)
{
    auto player1ID = m_playerManager->GetPlayer1();
    auto player2ID = m_playerManager->GetPlayer2();
    auto holeID = m_playerManager->GetHole();
    //Check if any player collides with hole
    if ((ID1 == player1ID && ID2 == holeID) || (ID1 == holeID && ID2 == player1ID))
    {
        printf("Player1 Scored");
        App::PlaySoundW("assets/GolfHole.wav");
        m_eventManager->Notify("EmitParticles", FVector2(0.5f, 0.5f), 100);

        m_playerManager->SetPlayer1Complete(true);
        m_registry->GetComponent<ModelComponent>(player1ID).layer = -1;
        m_registry->RemoveComponent<ColliderComponent>(player1ID);

        m_playerManager->SwapTurn();
    }
    else if ((ID1 == player2ID && ID2 == holeID) || (ID1 == holeID && ID2 == player2ID))
    {
        printf("Player2 Scored");
        App::PlaySoundW("assets/GolfHole.wav");
        m_eventManager->Notify("EmitParticles", FVector2(0.5f, 0.5f), 100);

        m_playerManager->SetPlayer2Complete(true);
        m_registry->GetComponent<ModelComponent>(player2ID).layer = -1;
        m_registry->RemoveComponent<ColliderComponent>(player2ID);

        m_playerManager->SwapTurn();
    }

    printf("Collision between %d and %d\n", ID1, ID2);
}

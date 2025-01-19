#include "stdafx.h"
#include "EntityFactory.h"
#include "Game/Core/Entity.h"
#include "Components.h"

EntityFactory::EntityFactory(Registry *registry) : m_registry(registry)
{
}

Entity EntityFactory::CreateGolfBallOne(const FVector3 &position)
{
    Entity player = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(player, position, FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<PlayerComponent>(player, position);
    m_registry->AddComponent<ModelComponent>(player, "SphereOBJ", FVector3(0.5f, 0.0f, 1.0f),1);
    m_registry->AddComponent<RigidBodyComponent>(player, 0.3, 0.3);
    m_registry->AddComponent<ColliderComponent>(player, ColliderType::Sphere, true, false, 0.8, 0.5f, 0.7f);
    // m_registry->AddComponent<BoxBoundsComponent>(player, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<CameraFollowComponent>(player, FVector3(0.0f, 5.0f, -30.0f), 1.0);
    m_registry->AddComponent<SphereBoundsComponent>(player, 1.0f);
    return player;
}

Entity EntityFactory::CreateGolfBallTwo(const FVector3 &position)
{
    Entity player2 = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(player2, position, FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<PlayerComponent>(player2, position);
    m_registry->AddComponent<ModelComponent>(player2, "SphereOBJ", FVector3(1.0f, 0.0f, 0.5f), 1);
    m_registry->AddComponent<RigidBodyComponent>(player2, 0.3, 0.3);
    m_registry->AddComponent<ColliderComponent>(player2, ColliderType::Sphere, true, false, 0.8, 0.5f, 0.7f);
    // m_registry->AddComponent<BoxBoundsComponent>(player, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<CameraFollowComponent>(player2, FVector3(0.0f, 5.0f, -30.0f), 1.0);
    m_registry->AddComponent<SphereBoundsComponent>(player2, 1.0f);
    return player2;
}

Entity EntityFactory::CreateDynamicSphere(const FVector3 &position, float radius)
{
    Entity sphere = m_registry->CreateEntity();

    // Add TransformComponent with the provided position, default rotation, and scale
    m_registry->AddComponent<TransformComponent>(sphere, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 FVector3(1.0f, 1.0f, 1.0f)                    // Scale
    );

    // Add ModelComponent with the model name "SphereOBJ"
    m_registry->AddComponent<ModelComponent>(sphere, "SphereOBJ");

    // Add RigidBodyComponent with linear and angular drag of 0.5f each
    m_registry->AddComponent<RigidBodyComponent>(sphere, 0.5f, 0.5f);

    // Add ColliderComponent as a dynamic Sphere collider
    m_registry->AddComponent<ColliderComponent>(sphere, ColliderType::Sphere, true);

    // Add SphereBoundsComponent with the specified radius
    m_registry->AddComponent<SphereBoundsComponent>(sphere, radius);

    return sphere;
}

Entity EntityFactory::CreateStaticSphere(const FVector3 &position, float radius)
{
    Entity sphere = m_registry->CreateEntity();

    // Add TransformComponent with the provided position, default rotation, and scale
    m_registry->AddComponent<TransformComponent>(sphere, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 FVector3(1.0f, 1.0f, 1.0f)                    // Scale
    );

    // Add ModelComponent with the model name "SphereOBJ"
    m_registry->AddComponent<ModelComponent>(sphere, "SphereOBJ");

    // Static objects typically do not require RigidBodyComponents or have them configured differently
    // Add ColliderComponent as a static Sphere collider
    m_registry->AddComponent<ColliderComponent>(sphere, ColliderType::Sphere, false);

    // Add SphereBoundsComponent with the specified radius
    m_registry->AddComponent<SphereBoundsComponent>(sphere, radius);

    return sphere;
}

Entity EntityFactory::CreateDynamicBox(const FVector3 &position, const FVector3 &extents)
{
    Entity box = m_registry->CreateEntity();

    // Add TransformComponent with the provided position, default rotation, and scale
    m_registry->AddComponent<TransformComponent>(box, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 FVector3(1.0f, 1.0f, 1.0f)                 // Scale
    );

    // Add ModelComponent with the model name "BoxOBJ"
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ");

    // Add RigidBodyComponent with linear and angular drag of 0.5f each
    m_registry->AddComponent<RigidBodyComponent>(box, 0.5f, 0.5f);

    // Add ColliderComponent as a dynamic Box collider
    m_registry->AddComponent<ColliderComponent>(box, ColliderType::Box, true);

    // Add BoxBoundsComponent with the specified extents
    m_registry->AddComponent<BoxBoundsComponent>(box, extents);

    return box;
}

// Creates a static Box entity with the specified position and extents
Entity EntityFactory::CreateStaticBox(const FVector3 &position, const FVector3 &extents)
{
    Entity box = m_registry->CreateEntity();

    // Add TransformComponent with the provided position, default rotation, and scale
    m_registry->AddComponent<TransformComponent>(box, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 FVector3(1.0f, 1.0f, 1.0f)                 // Scale
    );

    // Add ModelComponent with the model name "BoxOBJ"
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ");

    // Static objects typically do not require RigidBodyComponents or have them configured differently
    // Add ColliderComponent as a static Box collider
    m_registry->AddComponent<ColliderComponent>(box, ColliderType::Box, false);

    // Add BoxBoundsComponent with the specified extents
    m_registry->AddComponent<BoxBoundsComponent>(box, extents);

    return box;
}

Entity EntityFactory::CreateGrassBox(const FVector3 &position, const FVector3 &scale, const FVector3 &color )
{
    Entity box = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(box, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 scale               // Scale
    );
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ",color);
    m_registry->AddComponent<ColliderComponent>(box, ColliderType::Box, false,false,0.2,0.8,0.6);
    m_registry->AddComponent<BoxBoundsComponent>(box, FVector3(1.0f,1.0f,1.0f));
    return box;
}

Entity EntityFactory::CreateFlag(const FVector3 &position)
{
    Entity flag = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(flag, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 FVector3(1.0f, 1.0f, 1.0f)                  // Scale
    );
    m_registry->AddComponent<ModelComponent>(flag, "FlagOBJ");
    return flag;
}

Entity EntityFactory::CreateArrow(Entity followTarget)
{
    Entity arrow = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(arrow, FVector3(0.0f, 0.0f, 0.0f), FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ModelComponent>(arrow, "ArrowOBJ");
    m_registry->AddComponent<ArrowComponent>(arrow, followTarget);
    return arrow;
}

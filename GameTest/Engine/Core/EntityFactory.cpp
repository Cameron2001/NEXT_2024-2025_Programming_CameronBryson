#include "stdafx.h"
#include "EntityFactory.h"
#include "Engine/Core/Entity.h"
#include "Components.h"
#include <Game/GameComponents.h>

EntityFactory::EntityFactory(Registry *registry) : m_registry(registry)
{
}

Entity EntityFactory::CreatePlayer(const FVector3 &position)
{
    Entity player = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(player, FVector3(1.5f, 0.8f, -20.0f), FVector3(0.0f, 0.0f, 0.0f),
                                                 FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<PlayerComponent>(player, 2.0f);
    m_registry->AddComponent<ModelComponent>(player, "CubeOBJ");
    m_registry->AddComponent<RigidBodyComponent>(player, 0.5, 0.5);
    m_registry->AddComponent<ColliderComponent>(player, ColliderType::Box, true);
    m_registry->AddComponent<BoxBoundsComponent>(player, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<CameraFollowComponent>(player, FVector3(0.0f,0.0f, 20.0f),0.3);
    // m_registry->AddComponent<SphereBoundsComponent>(player, 1.0f);
    return Entity();
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

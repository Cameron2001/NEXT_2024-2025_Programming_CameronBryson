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
    m_registry->AddComponent<RigidBodyComponent>(player, 0.3,0.5, 0.5);
    m_registry->AddComponent<ColliderComponent>(player, ColliderType::Sphere, true, false, 0.8, 0.5f, 0.7f);
    // m_registry->AddComponent<BoxBoundsComponent>(player, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<CameraFollowComponent>(player, FVector3(0.0f, 20.0f, 50.0f), 0.8);
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
    m_registry->AddComponent<RigidBodyComponent>(player2, 0.5,0.4, 0.6);
    m_registry->AddComponent<ColliderComponent>(player2, ColliderType::Sphere, true, false, 0.8, 0.5f, 0.7f);
    // m_registry->AddComponent<BoxBoundsComponent>(player, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<CameraFollowComponent>(player2, FVector3(0.0f, 20.0f, 50.0f), 0.8);
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
    m_registry->AddComponent<RigidBodyComponent>(sphere,1.0f, 0.5f, 0.5f);

    // Add ColliderComponent as a dynamic Sphere collider
    m_registry->AddComponent<ColliderComponent>(sphere, ColliderType::Sphere, true);

    // Add SphereBoundsComponent with the specified radius
    m_registry->AddComponent<SphereBoundsComponent>(sphere, radius);

    return sphere;
}




// Creates a static Box entity with the specified position and extents
Entity EntityFactory::CreateStaticBox(const FVector3 &position, const FVector3 &extents, const FVector3 &rotation, const FVector3& color)
{
    Entity box = m_registry->CreateEntity();

    m_registry->AddComponent<TransformComponent>(box, position, rotation, 
                                                 extents                 // Scale
    );
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ", color);


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

Entity EntityFactory::CreateInvisibleBoxCollider(const FVector3 &position, const FVector3 &extents, const FVector3& rotation, const float elasticity, const float staticFriction, const float dynamicFriction)
{
    Entity collider = m_registry->CreateEntity();
    m_registry->AddComponent<TransformComponent>(collider, position, rotation, // Rotation
                                                 extents                      // Scale
    );
    m_registry->AddComponent<ColliderComponent>(collider, ColliderType::Box, false, true, elasticity,staticFriction, dynamicFriction);
    m_registry->AddComponent<BoxBoundsComponent>(collider, FVector3(1.0,1.0,1.0));
    return collider;
}

Entity EntityFactory::CreateBouncyBox(const FVector3 &position, const FVector3 &extents)
{
    Entity box = m_registry->CreateEntity();

    m_registry->AddComponent<TransformComponent>(box, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 extents);                                  // Scale
    m_registry->AddComponent<BoxBoundsComponent>(box, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ", FVector3(0.2f, 1.0f, 0.2f));
    m_registry->AddComponent<ColliderComponent>(box, ColliderType::Box, false, false, 2.0, 0.1f, 0.1f);
    return box;
}

Entity EntityFactory::CreateBouncySphere(const FVector3 &position, float radius)
{
    Entity sphere = m_registry -> CreateEntity();
    m_registry->AddComponent<TransformComponent>(sphere, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 FVector3(radius, radius, radius)                    // Scale
    );
    m_registry->AddComponent<ModelComponent>(sphere, "SphereOBJ", FVector3(0.2f, 1.0f, 0.2f));
    m_registry->AddComponent<ColliderComponent>(sphere, ColliderType::Sphere, false, false, 2.0, 0.1f, 0.1f);

    return sphere;
}

Entity EntityFactory::CreateSandBox(const FVector3 &position, const FVector3 &extents)
{
    Entity box = m_registry->CreateEntity();

    m_registry->AddComponent<TransformComponent>(box, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 extents);                                  // Scale
    m_registry->AddComponent<BoxBoundsComponent>(box, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ", FVector3(1.0f, 1.0f, 0.0f));
    m_registry->AddComponent<ColliderComponent>(box, ColliderType::Box, false, false, 0.2, 0.99f, 0.99f);
    return box;
}

Entity EntityFactory::CreateIceBox(const FVector3 &position, const FVector3 &extents)
{
    Entity box = m_registry->CreateEntity();

    m_registry->AddComponent<TransformComponent>(box, position, FVector3(0.0f, 0.0f, 0.0f), // Rotation
                                                 extents);                                  // Scale
    m_registry->AddComponent<BoxBoundsComponent>(box, FVector3(1.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ModelComponent>(box, "CubeOBJ", FVector3(0.0f, 1.0f, 1.0f));
    m_registry->AddComponent<ColliderComponent>(box, ColliderType::Box, false, false, 0.4, 0.1f, 0.1f);
    return box;
}

void EntityFactory::CreateStaticBoxGrid(int rows, int cols, const FVector3 &startPosition, float spacing,
                                        const FVector3 &extents, const FVector3 &color, const float elasticity,
                                              const float staticFriction, const float dynamicFriction)
{
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            FVector3 position = startPosition;
            position.x += col * spacing;
            position.z += row * spacing;

            // Create the static box without an individual collider
            CreateStaticBox(position, extents, FVector3(0.0f, 0.0f, 0.0f), color);
        }
    }
    FVector3 largeColliderPosition;
    largeColliderPosition.x = startPosition.x + (cols * spacing) / 2.0f - spacing / 2.0f;
    largeColliderPosition.y = startPosition.y;
    largeColliderPosition.z = startPosition.z + (rows * spacing) / 2.0f - spacing / 2.0f;

    FVector3 largeColliderExtents;
    largeColliderExtents.x = (cols * spacing) / 2.0f;
    largeColliderExtents.y = extents.y; // Assuming the collider height matches the box height
    largeColliderExtents.z = (rows * spacing) / 2.0f;

    // Create a single large invisible collider encompassing the entire grid
    CreateInvisibleBoxCollider(largeColliderPosition, largeColliderExtents, FVector3(0, 0, 0), elasticity,
                               staticFriction,
                               dynamicFriction);
}

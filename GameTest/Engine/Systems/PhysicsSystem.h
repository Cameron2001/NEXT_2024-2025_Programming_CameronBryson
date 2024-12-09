#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include <Engine/Storage/Registry.h>

class PhysicsSystem
{
public:
    PhysicsSystem(Registry* registry);
    ~PhysicsSystem() = default;
    void Init();
    void Update(float dt);
    void Shutdown();

private:
    Registry *m_registry;
};

//Read:
//transform
//rigidbody

//Write:
//transform
//rigidbody
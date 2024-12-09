#pragma once
#include <memory>
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Game/GameComponents.h"
#include <Engine/Storage/Registry.h>

class PlayerSystem
{
public:
    PlayerSystem(Registry* registry);
    ~PlayerSystem() = default;
    void Init();
    void Update(float dt);

private:
    Registry *m_registry;

};

//Read:
//Player

//Write:
//rigidbody
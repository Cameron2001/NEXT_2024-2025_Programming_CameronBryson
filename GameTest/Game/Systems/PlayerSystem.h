#pragma once
#include <Game/Storage/Registry.h>

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
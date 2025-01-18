#pragma once
#include <Game/Storage/Registry.h>
#include "Game/Managers/EventManager.h"
#include "Game/Managers/PlayerManager.h"

class PlayerSystem
{
public:
    PlayerSystem(Registry* registry,EventManager* eventManager, PlayerManager* playerManager);
    ~PlayerSystem() = default;
    void Init();
    void Update(float dt);
    void OnCollision(unsigned int ID1, unsigned int ID2);

private:
    Registry *m_registry;
    EventManager *m_eventManager;
    PlayerManager *m_playerManager;
    

};
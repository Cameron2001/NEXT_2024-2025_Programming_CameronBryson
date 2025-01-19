#pragma once
#include "Game/Managers/EventManager.h"
#include "Game/Managers/PlayerManager.h"
#include <Game/Core/Components.h>
#include <Game/Storage/Registry.h>

class PlayerSystem
{
public:
    PlayerSystem(Registry* registry,EventManager* eventManager, PlayerManager* playerManager);
    ~PlayerSystem() = default;
    void Init();
    void Update(float dt);
    void OnCollision(unsigned int ID1, unsigned int ID2);
    void SetScaleTextEntity(Entity scaleTextEntity);

private:
    Registry *m_registry;
    EventManager *m_eventManager;
    PlayerManager *m_playerManager;
    bool shotFired = false;

    bool previousSpace = false;
    View<ArrowComponent, ModelComponent, TransformComponent> m_arrowView;
    float forceScale = 5.0f;
    Entity m_scaleTextEntity;
    

};
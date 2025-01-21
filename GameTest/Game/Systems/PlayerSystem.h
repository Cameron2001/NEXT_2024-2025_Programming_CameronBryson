#pragma once
#include "Game/Managers/EventManager.h"
#include "Game/Managers/PlayerManager.h"
#include <Game/Core/Components.h>
#include <Game/Storage/Registry.h>
#include "Game/Graphics/Camera.h"

class PlayerSystem
{
public:
    PlayerSystem(Registry* registry,EventManager* eventManager, PlayerManager* playerManager, Camera* camera);
    ~PlayerSystem() = default;
    void Init();
    void Update(float dt);
    void OnCollision(unsigned int ID1, unsigned int ID2);
    void SetScaleTextEntity(Entity scaleTextEntity);

private:
    Registry *m_registry;
    EventManager *m_eventManager;
    PlayerManager *m_playerManager;
    Camera *m_camera;
    FVector3 m_shotDirection = FVector3(0.0f, 0.0f, 0.0f);
    View<ArrowComponent, ModelComponent, TransformComponent> m_arrowView;
    bool shotFired = false;
    bool previousSpace = false;
    float forceScale = 5.0f;
    Entity m_scaleTextEntity;
    float totalYaw = 180;
    float totalPitch = 0;

    

};
#pragma once
#include "Game/Storage/Registry.h"
#include "Game/Core/Components.h"
#include <Game/Graphics/Camera.h>
#include "Game/Managers/PlayerManager.h"
class CameraSystem
{
  public:
    CameraSystem(Registry *registry, PlayerManager* playerManager, Camera *camera);
    void Update(float dt);

  private:
    Registry *m_registry;
    PlayerManager *m_playerManager;
    Camera *m_camera;
    View<CameraFollowComponent, TransformComponent> m_view;
    int m_selectedFollowIndex = 0;
};

#pragma once
#include "Game/Storage/Registry.h"
#include "Game/Core/Components.h"
#include <Game/Graphics/Camera.h>
#include "Game/Storage/Registry.h"
class CameraSystem
{
  public:
    CameraSystem(Registry *registry, Camera *camera);
    void Update(float dt);

  private:
    Registry *m_registry;
    Camera *m_camera;
    View<CameraFollowComponent, TransformComponent> m_view;
};

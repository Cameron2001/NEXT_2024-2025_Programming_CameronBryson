#pragma once
#include "Engine/Storage/Registry.h"
#include "Engine/Core/Components.h"
#include <Engine/Graphics/Camera.h>
#include "Engine/Storage/Registry.h"
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

#include "stdafx.h"
#include "CameraSystem.h"

void CameraSystem::Update(float dt)
{
    m_view.Update();
    m_view.ForEach([&](Entity entity, CameraFollowComponent &cameraFollow, TransformComponent &transform) {
        // Lerp camera position based on the camerfollowcomponent position and offset and the smoothSpeed
    });
}

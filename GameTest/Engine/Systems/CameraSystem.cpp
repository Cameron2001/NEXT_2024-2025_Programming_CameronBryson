#include "stdafx.h"
#include "CameraSystem.h"
#include "Engine/Math/MathUtil.h"
#include <App/app.h>

CameraSystem::CameraSystem(Registry *registry, Camera *camera) : m_registry(registry), m_camera(camera), m_view(registry)
{
}


void CameraSystem::Update(float dt)
{
    m_view.Update();
    m_view.ForEach([&](Entity entity, CameraFollowComponent &cameraFollow, TransformComponent &transform) {
        m_camera->SetPosition(MathUtil::Lerp(m_camera->GetPosition(), transform.Position + cameraFollow.offset,
                                             cameraFollow.smoothSpeed * dt));
    });

}

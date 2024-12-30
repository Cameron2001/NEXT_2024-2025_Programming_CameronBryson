#pragma once
#include <memory>
#include <vector>
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Storage/ComponentArray.h"
#include "Engine/Core/Components.h"
#include "Engine/Graphics/Camera.h"
#include <Engine/Storage/Registry.h>

class RenderSystem
{
public:
    RenderSystem(Registry* registry, GraphicsManager* graphicsManager, Camera* camera);
    ~RenderSystem() = default;
    void Init();
    void Render();
    void LateRender();
    void Shutdown();

private:
    Registry *m_registry;
    GraphicsManager *m_graphicsManager;
    Camera *m_camera;
};


//Read:
//GraphicsManager
//Camera
//TransformComponent
//ModelComponent
//TextComponent

//Write:
//None
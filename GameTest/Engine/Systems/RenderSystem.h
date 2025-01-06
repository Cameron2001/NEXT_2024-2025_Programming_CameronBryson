#pragma once
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/HiddenLineRemoval.h"
#include "Engine/Managers/GraphicsManager.h"
#include <concurrent_vector.h>
#include <Engine/Graphics/Edge.h>
#include <Engine/Graphics/Triangle.h>
#include <Engine/Storage/Registry.h>
#include <vector>

class RenderSystem
{
  public:
    RenderSystem(Registry *registry, GraphicsManager *graphicsManager, Camera *camera);
    ~RenderSystem() = default;
    void Init();
    void Update();
    void Render();
    void LateRender();
    void Shutdown();

  private:
    Registry *m_registry;
    GraphicsManager *m_graphicsManager;
    Camera *m_camera;
    HiddenLineRemoval m_hiddenLineRemoval;

    // Pre-allocated buffers
    //Should probably change concurrent vecctor to combinable vector
    concurrency::concurrent_vector<Triangle2D> m_triangles;
    std::vector<Triangle2D> m_triangleList;
    std::vector<Edge2D> m_visibleSegments;
};

// Read:
// GraphicsManager
// Camera
// TransformComponent
// ModelComponent
// TextComponent

// Write:
// None

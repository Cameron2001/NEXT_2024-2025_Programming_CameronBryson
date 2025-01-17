#pragma once
#include "Game/Graphics/Camera.h"
#include "Game/Graphics/HiddenLineRemoval.h"
#include "Game/Managers/GraphicsManager.h"
#include <concurrent_vector.h>
#include <Game/Core/Components.h>
#include <Game/Graphics/Edge.h>
#include <Game/Graphics/Triangle.h>
#include <Game/Math/Vector3.h>
#include <Game/Storage/Registry.h>
#include <Game/Storage/View.h>
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
    bool RejectTriangle(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2);
    Registry *m_registry;
    View<TransformComponent, ModelComponent> m_modelView;
    View<TextComponent> m_textView;
    GraphicsManager *m_graphicsManager;
    Camera *m_camera;
    HiddenLineRemoval m_hiddenLineRemoval;

    // Pre-allocated buffers
    // Should probably change concurrent vecctor to combinable vector
    concurrency::combinable<std::vector<Triangle2D>> m_triangles;
    // concurrency::concurrent_vector<Triangle2D> m_triangles;
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

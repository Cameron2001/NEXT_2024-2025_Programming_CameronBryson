#pragma once
#include "Engine/Graphics/Camera.h"
#include "Engine/Graphics/HiddenLineRemoval.h"
#include "Engine/Managers/GraphicsManager.h"
#include <concurrent_vector.h>
#include <Engine/Core/Components.h>
#include <Engine/Graphics/Edge.h>
#include <Engine/Graphics/Triangle.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Storage/Registry.h>
#include <Engine/Storage/View.h>
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

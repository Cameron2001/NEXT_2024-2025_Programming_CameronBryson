#pragma once
#include "Game/Storage/Registry.h"

#include "Game/Managers/GraphicsManager.h"
#include "Game/Managers/EventManager.h"

#include "Game/Systems/CollisionSystem.h"
#include "Game/Systems/PhysicsSystem.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/ParticleSystem.h"
#include "Game/Systems/UISystem.h"
#include "Game/Core/EntityFactory.h"
#include <Game/Managers/PlayerManager.h>
class Scene
{
  public:
    Scene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager, std::shared_ptr<PlayerManager> scoreManager);
    virtual ~Scene() = default;

    virtual void Init();
    virtual void LateInit();
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Render();
    virtual void LateRender();
    virtual void Shutdown();
    virtual void LateShutdown();

  private:
  protected:

    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<Camera> m_camera;

    std::shared_ptr<GraphicsManager> m_graphicsManager;
    std::shared_ptr<EventManager> m_eventManager;
    std::shared_ptr<PlayerManager> m_playerManager;

    std::shared_ptr<RenderSystem> m_renderSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;

};

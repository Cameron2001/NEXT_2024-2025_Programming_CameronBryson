#pragma once
#include "Game/Storage/Registry.h"

#include "Game/Managers/GraphicsManager.h"
#include "Game/Managers/EventManager.h"

#include "Game/Systems/CollisionSystem.h"
#include "Game/Systems/PhysicsSystem.h"
#include "Game/Systems/RenderSystem.h"
#include "Game/Systems/ParticleSystem.h"
#include "Game/Systems/CameraSystem.h"
#include "Game/Systems/UISystem.h"
#include "Game/Core/EntityFactory.h"
#include <Game/Managers/PlayerManager.h>
class Scene
{
  public:
    Scene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager, std::shared_ptr<PlayerManager> scoreManager);
    virtual ~Scene() = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;
    Scene(Scene &&) = delete;
    Scene &operator=(Scene &&) = delete;
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
    // Who do I want to have access to the members
    // Should I create an interface to interact with the members
    // Who should be able to access that interface

    // Registry
    std::unique_ptr<Registry> m_registry;
    std::unique_ptr<Camera> m_camera;

    // Managers
    // Managers are for storing and getting data
    // Should move managers that might benefit from being passed to a new scene out
    // or make it shared pointers that can be passed to new scene and lifetime will be extended
    std::shared_ptr<GraphicsManager> m_graphicsManager;
    std::shared_ptr<EventManager> m_eventManager;
    std::shared_ptr<PlayerManager> m_playerManager;

    // Systems
    // Systems are for processing data and editing data
    // Systems should not store data
    // Systems should not have access to other systems
    // They should have to interface with the manager or with events
    // Should we seperate read only systems with systems that edit data
    // Lifetime of systems shouldnt really matter because they are all the same. No data. Just processing
    // Could be made static if changed a bit
    // Currently systems either need access to the registry or only to the specific components it needs
    std::shared_ptr<RenderSystem> m_renderSystem;
    
    // Rendersystem only reads data
    // Physics system reads and edits data
    // Collision system reads and edits data
    //  COllision system could be split into two. One that detects collision and one that resolves collision
};

#pragma once
#include "Engine/Storage/Registry.h"

#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/CollisionManager.h"
#include "Engine/Managers/GraphicsManager.h"

#include "Engine/Systems/CollisionDetectionSystem.h"
#include "Engine/Systems/PhysicsSystem.h"
#include "Engine/Systems/RenderSystem.h"

class Scene
{
public:
    Scene();
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
    //Who do I want to have access to the members
    //Should I create an interface to interact with the members
    //Who should be able to access that interface

    //Registry
    std::shared_ptr<Registry> m_registry;
    std::shared_ptr<Camera> m_camera;

    //Managers
    //Managers are for storing and getting data
    //Should move managers that might benefit from being passed to a new scene out
    //or make it shared pointers that can be passed to new scene and lifetime will be extended
    std::shared_ptr<AudioManager> m_audioManager;
    std::shared_ptr<GraphicsManager> m_graphicsManager;
    std::shared_ptr<CollisionManager> m_collisionManager;


    //Systems
    //Systems are for processing data and editing data
    //Systems should not store data
    //Systems should not have access to other systems
    //They should have to interface with the manager or with events
    //Should we seperate read only systems with systems that edit data
    //Lifetime of systems shouldnt really matter because they are all the same. No data. Just processing
    //Could be made static if changed a bit
    //Currently systems either need access to the registry or only to the specific components it needs
    RenderSystem m_renderSystem;
    CollisionDetectionSystem m_collisionSystem;
    PhysicsSystem m_physicsSystem;


    //Rendersystem only reads data
    //Physics system reads and edits data
    //Collision system reads and edits data
    // COllision system could be split into two. One that detects collision and one that resolves collision

};
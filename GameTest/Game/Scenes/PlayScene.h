#pragma once
#include "Game/Core/Scene.h"
#include "Game/Systems/PlayerSystem.h"
#include <memory>

class PlayScene : public Scene
{
  public:
    PlayScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
              std::shared_ptr<PlayerManager> scoreManager);
    ~PlayScene() override = default;
    void Init() override;
    void LateInit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void Render() override;
    void LateRender() override;
    void Shutdown() override;
    void LateShutdown() override;
    void Test(int value);
    void CollisionEvent(unsigned int ID1, unsigned int ID2);

  protected:
    std::shared_ptr<PlayerSystem> m_playerSystem;
    std::shared_ptr<CollisionSystem> m_collisionSystem;
    
    std::shared_ptr<ParticleSystem> m_particleSystem;
    std::shared_ptr<CameraSystem> m_cameraSystem;
    std::shared_ptr<UISystem> m_uiSystem;
    std::shared_ptr<EntityFactory> m_entityFactory;
};

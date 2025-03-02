#pragma once
#include "Game/Core/Scene.h"
#include "Game/Systems/MenuSystem.h"
class MainMenuScene : public Scene
{
  public:
    MainMenuScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                  std::shared_ptr<PlayerManager> scoreManager);
    ~MainMenuScene() override = default;
    void Init() override;
    void LateInit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void Render() override;
    void LateRender() override;
    void Shutdown() override;
    void LateShutdown() override;

  private:
    std::shared_ptr<MenuSystem> m_menuSystem;

};

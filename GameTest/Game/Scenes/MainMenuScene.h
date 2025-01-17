#pragma once
#include "Game/Core/Scene.h"
class MainMenuScene : public Scene
{
  public:
    MainMenuScene(std::shared_ptr<AudioManager> audioManager, std::shared_ptr<GraphicsManager> graphicsManager,
                  std::shared_ptr<EventManager> eventManager);
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
  protected:
};

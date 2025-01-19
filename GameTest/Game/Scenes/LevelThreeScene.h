#pragma once
#include "Game/Scenes/PlayScene.h"
class LevelThreeScene : public PlayScene
{
  public:
    LevelThreeScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                    std::shared_ptr<PlayerManager> scoreManager);
    ~LevelThreeScene() override = default;
    void Init() override;
    void LateInit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void Render() override;
    void LateRender() override;
    void Shutdown() override;
    void LateShutdown() override;
  private:
    void BuildLevelThree();
};

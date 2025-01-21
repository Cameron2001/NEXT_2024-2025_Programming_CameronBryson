#pragma once
#include "Game/Scenes/PlayScene.h"
class LevelOneScene : public PlayScene
{
  public:
    LevelOneScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                  std::shared_ptr<PlayerManager> scoreManager);
    ~LevelOneScene() override = default;
    void Init() override;
    void LateInit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void Render() override;
    void LateRender() override;
    void Shutdown() override;
    void LateShutdown() override;
  private:
    void BuildLevelOne();
    void BuildPlayers();
    void BuildUI();
    void BuildGrassArea();
    void BuildGrassBoundaries();
    void BuildIceArea();
    void BuildIceBoundaries();
    void BuildBouncyBoxes();
    void BuildRamps();
    void BuildCentralGrassArea();
    void BuildSandBorders();
    void CreateBoundary(const FVector3 &position, float height, float elasticity, float friction);
};

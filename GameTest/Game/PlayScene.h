#pragma once
#include "Engine/Core/Scene.h"
#include "Game/PlayerSystem.h"
#include <memory>

class PlayScene : public Scene, public std::enable_shared_from_this<PlayScene>
{
  public:
    PlayScene();
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

  private:
    PlayerSystem m_playerSystem;
};

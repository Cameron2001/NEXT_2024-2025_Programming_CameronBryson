#pragma once
#include "Engine/Core/Scene.h"
class MainMenuScene : public Scene
{
  public:
    MainMenuScene();
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

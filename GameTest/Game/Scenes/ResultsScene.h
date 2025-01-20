#pragma once
#include "Game/Core/Scene.h"
#include <Game/Systems/ResultsSystem.h>
class ResultsScene : public Scene
{
  public:
    ResultsScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                 std::shared_ptr<PlayerManager> scoreManager);
    ~ResultsScene() override = default;

    virtual void Init() override;
    virtual void LateInit() override;
    virtual void Update(float dt) override;
    virtual void LateUpdate(float dt) override;
    virtual void Render() override;
    virtual void LateRender() override;
    virtual void Shutdown() override;
    virtual void LateShutdown() override;
  private:
    void BuildResults();
    std::shared_ptr<ResultsSystem> m_resultsSystem;
};

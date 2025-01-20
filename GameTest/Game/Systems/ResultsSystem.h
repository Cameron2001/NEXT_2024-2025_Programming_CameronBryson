#pragma once
#include "Game/Storage/Registry.h"
#include "Game/Managers/PlayerManager.h" 
#include "Game/Managers/EventManager.h"
class ResultsSystem
{
  public:
    ResultsSystem(Registry *registry, PlayerManager *playerManager, EventManager* eventManager);
    void LateInit();
    void Update(float dt);

    void SetPlayer1ScoreText(Entity entity);
    void SetPlayer2ScoreText(Entity entity);
    void SetWinnerText(Entity entity);
  private:
    Registry *m_registry;
    PlayerManager *m_playerManager;
    EventManager *m_eventManager;
    Entity player1ScoreText;
    Entity player2ScoreText;
    Entity winnerText;




};

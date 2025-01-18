#pragma once
#include "Game/Core/Entity.h"
#include "Game/Storage/Registry.h"
#include "Game/Managers/PlayerManager.h"
class UISystem
{
	//Use this class to update ui elements
  public:
    UISystem(Registry *registry, PlayerManager *playerManager);
    void SetScoreTextEntities(Entity player1ScoreTextEntity, Entity player2ScoreTextEntity);
    void Update(float dt);

  private:
    Registry *m_registry;
    PlayerManager *m_playerManager;
    Entity m_player1ScoreTextEntity;
    Entity m_player2ScoreTextEntity;
};

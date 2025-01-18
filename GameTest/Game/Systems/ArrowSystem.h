#pragma once
#include "Game/Storage/Registry.h"
#include <Game/Core/Components.h>
#include "Game/Managers/PlayerManager.h"
class ArrowSystem
{
	//We need this system to control the arrow.
	// The arrow will be a model of an arrow. 
	// When its a players turn the arrow will be visable next to the players ball
	//The arrow will point in the direction that the ball will shoot when the player hits the ball
	//We want the player to be able to adjust the direction of the arrow
    // We dont need to see the arrow when its not the players turn
    //  We need to be able to rotate the arrow
  public:
    ArrowSystem(Registry *registry, PlayerManager* playerManager);
    void SetScaleTextEntity(Entity scaleTextEntity);
    void Update(float dt);
  private:
    Registry *m_registry;
    PlayerManager *m_playerManager;
    Entity m_scaleTextEntity;


    bool previousSpace = false;
    View<ArrowComponent, ModelComponent, TransformComponent> m_arrowView;
    float forceScale = 5.0f;


};

#pragma once
#include "Game/Managers/EventManager.h"
class MenuSystem
{
  public:
    MenuSystem(EventManager* eventManager);
    ~MenuSystem();
    void Update(float dt);

  private:
    EventManager *m_eventManager;
};

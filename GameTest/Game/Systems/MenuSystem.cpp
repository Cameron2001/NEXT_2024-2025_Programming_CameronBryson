#include "stdafx.h"
#include "MenuSystem.h"
#include <App/app.h>

MenuSystem::MenuSystem(EventManager *eventManager): m_eventManager(eventManager)
{

}

MenuSystem::~MenuSystem()
{

}

void MenuSystem::Update(float dt)
{
    const bool One = App::IsKeyPressed('1');
    if (One)
    {
        m_eventManager->Notify("SceneChange", "LevelOne");
    }
}

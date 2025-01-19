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
    bool One = App::IsKeyPressed('1');
    bool Two = App::IsKeyPressed('2');
    bool Three = App::IsKeyPressed('3');
    if (One)
    {
        m_eventManager->Notify("SceneChange", "LevelOne");
    }
    if (Two)
    {
        m_eventManager->Notify("SceneChange", "LevelTwo");
    }
    if (Three)
    {
        m_eventManager->Notify("SceneChange", "LevelThree");
    }
}

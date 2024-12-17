///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <math.h>
#include <windows.h>
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Engine/Managers/SceneManager.h"
#include "Game/PlayScene.h"
float fps;
void Init()
{
    SceneManager::LoadScene<PlayScene>();
    SceneManager::GetCurrentScene().Init();
    SceneManager::GetCurrentScene().LateInit();
}

void Update(const float deltaTime)
{
    SceneManager::GetCurrentScene().Update(deltaTime);
    SceneManager::GetCurrentScene().LateUpdate(deltaTime);
    fps = 1.0f / deltaTime * 1000.0f;
}

void Render()
{
    SceneManager::GetCurrentScene().Render();
    SceneManager::GetCurrentScene().LateRender();
    std::string fpsStr = std::to_string(fps);
    fpsStr = "FPS: " + fpsStr;
    App::Print(0.5, 0.5, fpsStr.c_str());
}
void Shutdown()
{
    SceneManager::GetCurrentScene().Shutdown();
    SceneManager::GetCurrentScene().LateShutdown();
}

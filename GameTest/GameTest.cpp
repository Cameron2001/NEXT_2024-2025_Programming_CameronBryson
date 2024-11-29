///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "Engine/Managers/SceneManager.h"
#include "Game/PlayScene.h"
void Init()
{
	SceneManager::LoadScene<PlayScene>();
	SceneManager::GetCurrentScene()->Init();
	SceneManager::GetCurrentScene()->LateInit();
}

void Update(const float deltaTime)
{
	SceneManager::GetCurrentScene()->Update(deltaTime);
	SceneManager::GetCurrentScene()->LateUpdate(deltaTime);
}

void Render()
{	
	SceneManager::GetCurrentScene()->Render();
	SceneManager::GetCurrentScene()->LateRender();

}
void Shutdown()
{
	SceneManager::GetCurrentScene()->Shutdown();
	SceneManager::GetCurrentScene()->LateShutdown();
}
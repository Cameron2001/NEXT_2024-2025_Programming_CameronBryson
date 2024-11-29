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
	SceneManager::Init();
	SceneManager::LateInit();
}

void Update(const float deltaTime)
{
	SceneManager::Update(deltaTime);
	SceneManager::LateUpdate(deltaTime);
}

void Render()
{	
	SceneManager::Render();
	SceneManager::LateRender();

}
void Shutdown()
{
	SceneManager::Shutdown();
	SceneManager::LateShutdown();
}
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
#include "App\app.h"
//------------------------------------------------------------------------
#include "Game/Managers/SceneManager.h"
#include "Game/Scenes/PlayScene.h"
#include "Game/Scenes/MainMenuScene.h"
#include "Game/Scenes/LevelOneScene.h"
#include "Game/Scenes/ResultsScene.h"
#include "Game/Core/Tests.h"
#include "Game/Core/Logger.h"

const float MAX_DT = 0.2f;
bool testsEnabled = true;
std::shared_ptr<SceneManager> sceneManager = std::make_shared<SceneManager>();
void Init()
{
    sceneManager->BindSceneChangeEvent();
    if (testsEnabled)
    {
        auto testResults = RunTests();
        for (const auto &testResult : testResults)
        {
            Logger::GetInstance().LogTestResult(testResult);
        }
        Logger::GetInstance().LogInfo("All tests completed.");
    }
    sceneManager->LoadScene<MainMenuScene>();

}

void Update(float deltaTime)
{
    deltaTime /= 1000.0f;
    float clampedDeltaTime = (std::min)(deltaTime, MAX_DT);
    sceneManager->GetCurrentScene()->Update(clampedDeltaTime);
    sceneManager->GetCurrentScene()->LateUpdate(clampedDeltaTime);
}

void Render()
{
    sceneManager->GetCurrentScene()->Render();
    sceneManager->GetCurrentScene()->LateRender();
    
}
void Shutdown()
{
    sceneManager->GetCurrentScene()->Shutdown();
    sceneManager->GetCurrentScene()->LateShutdown();
}

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
#include "Game/Core/Tests.h"
#include "Game/Core/Logger.h"
float fps = 0.0f;
float minFps = 999999.0f;
float maxFps = 0.0f;
float totalFps = 0.0f;
int frameCount = 0;
const float MAX_DT = 0.2f;
// Warm-Up Configuration
const int warmUpFrames = 5; // Number of frames to skip for minFps
int warmUpFrameCount = 0;   // Counter for warm-up frames
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

    // SceneManager::GetCurrentScene().Init();
    // SceneManager::GetCurrentScene().LateInit();
}

void Update(float deltaTime)
{
    deltaTime /= 1000.0f;
    float clampedDeltaTime = (std::min)(deltaTime, MAX_DT);
    sceneManager->GetCurrentScene()->Update(clampedDeltaTime);
    sceneManager->GetCurrentScene()->LateUpdate(clampedDeltaTime);
    // Calculate FPS
    if (deltaTime > 0.0f)
        fps = 1.0f / deltaTime;

    // Update FPS Metrics
    if (warmUpFrameCount >= warmUpFrames)
    {
        // Update minFps after warm-up
        if (fps < minFps)
            minFps = fps;
    }

    // Always update maxFps and totalFps
    if (fps > maxFps)
        maxFps = fps;

    totalFps += fps;
    frameCount++;

    // Increment warm-up frame counter
    if (warmUpFrameCount < warmUpFrames)
        warmUpFrameCount++;
}

void Render()
{
    sceneManager->GetCurrentScene()->Render();
    sceneManager->GetCurrentScene()->LateRender();
    // Calculate Average FPS
    float averageFps = (frameCount > 0) ? (totalFps / frameCount) : 0.0f;

    // Prepare FPS Strings
    std::string fpsStr = "FPS: " + std::to_string(fps);
    std::string minFpsStr = "Min FPS: " + std::to_string(minFps);
    std::string maxFpsStr = "Max FPS: " + std::to_string(maxFps);
    std::string avgFpsStr = "Avg FPS: " + std::to_string(averageFps);

    // Print FPS Metrics
    App::Print(100, 200, fpsStr.c_str());
    App::Print(100, 220, minFpsStr.c_str());
    App::Print(100, 240, maxFpsStr.c_str());
    App::Print(100, 260, avgFpsStr.c_str());
}
void Shutdown()
{
    sceneManager->GetCurrentScene()->Shutdown();
    sceneManager->GetCurrentScene()->LateShutdown();
}

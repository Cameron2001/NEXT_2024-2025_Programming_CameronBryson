#include "stdafx.h"
#include "ResultsScene.h"

ResultsScene::ResultsScene(std::shared_ptr<GraphicsManager> graphicsManager, std::shared_ptr<EventManager> eventManager,
                           std::shared_ptr<PlayerManager> scoreManager)
    : Scene(graphicsManager, eventManager, scoreManager), m_resultsSystem(std::make_shared<ResultsSystem>(m_registry.get(), m_playerManager.get(),eventManager.get()))
{
}

void ResultsScene::Init()
{
    BuildResults();
    Scene::Init();
}

void ResultsScene::LateInit()
{
    m_resultsSystem->LateInit();

    Scene::LateInit();
}

void ResultsScene::Update(float dt)
{
    m_resultsSystem->Update(dt);
    Scene::Update(dt);
}

void ResultsScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
}

void ResultsScene::Render()
{
    Scene::Render();
}

void ResultsScene::LateRender()
{
    Scene::LateRender();
}

void ResultsScene::Shutdown()
{
    Scene::Shutdown();
}

void ResultsScene::LateShutdown()
{
    Scene::LateShutdown();
}

void ResultsScene::BuildResults()
{
    auto entityPlayer1ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(entityPlayer1ScoreText,
                                            "Player 1 Score: " + std::to_string(m_playerManager->GetPlayer1Score()),
                                            FVector2(100.0f, 100.0f));
    auto entityPlayer2ScoreText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(entityPlayer2ScoreText,
                                            "Player 2 Score: " + std::to_string(m_playerManager->GetPlayer2Score()),
                                            FVector2(80,80));

    auto entityWinnerText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(entityWinnerText, "Winner: ",
                                            FVector2(100, 150));

    auto entityControlsText = m_registry->CreateEntity();
    m_registry->AddComponent<TextComponent>(entityControlsText, "Press SPACE to return to Main Menu", FVector2(76,210));

    m_resultsSystem->SetPlayer1ScoreText(entityPlayer1ScoreText);
    m_resultsSystem->SetPlayer2ScoreText(entityPlayer2ScoreText);
    m_resultsSystem->SetWinnerText(entityWinnerText);

}

#include "stdafx.h"
#include "ResultsSystem.h"
#include <Game/Core/Components.h>
#include <App/app.h>


ResultsSystem::ResultsSystem(Registry *registry, PlayerManager *playerManager, EventManager *eventManager)
    : m_registry(registry), m_playerManager(playerManager), m_eventManager(eventManager), player1ScoreText(0),
      player2ScoreText(0),
      winnerText(0)
{
}

void ResultsSystem::LateInit()
{
    const int player1Score = m_playerManager->GetPlayer1Score();
    const int player2Score = m_playerManager->GetPlayer2Score();

    if (player1Score < player2Score)
    {
        m_registry->GetComponent<TextComponent>(winnerText).text = "Player 1 Wins!";
    }
    else if (player1Score > player2Score)
    {
        m_registry->GetComponent<TextComponent>(winnerText).text = "Player 2 Wins!";
    }
    else
    {
        m_registry->GetComponent<TextComponent>(winnerText).text = "It's a tie!";
    }

    m_registry->GetComponent<TextComponent>(player1ScoreText).text = "Player 1: " + std::to_string(player1Score);
    m_registry->GetComponent<TextComponent>(player2ScoreText).text = "Player 2: " + std::to_string(player2Score);

}

void ResultsSystem::Update(float dt)
{
    const bool SPACE = App::IsKeyPressed(VK_SPACE);
    if (SPACE)
    {
        m_playerManager->ResetScores();
        m_eventManager->Notify("SceneChange", "MainMenu");
    }
}


void ResultsSystem::SetPlayer1ScoreText(const Entity entity)
{
    player1ScoreText = entity;
}

void ResultsSystem::SetPlayer2ScoreText(const Entity entity)
{
    player2ScoreText = entity;
}

void ResultsSystem::SetWinnerText(const Entity entity)
{
    winnerText = entity;
}


#include "stdafx.h"
#include "UISystem.h"
#include <Game/Core/Components.h>

UISystem::UISystem(Registry *registry, PlayerManager *playerManager)
    : m_registry(registry), m_playerManager(playerManager), m_player1ScoreTextEntity(0), m_player2ScoreTextEntity(0)
{
}

void UISystem::SetScoreTextEntities(const Entity player1ScoreTextEntity, const Entity player2ScoreTextEntity)
{
    m_player1ScoreTextEntity = player1ScoreTextEntity;
    m_player2ScoreTextEntity = player2ScoreTextEntity;
}

void UISystem::Update(float dt)
{
    int player1Score = m_playerManager->GetPlayer1Score();
    int player2Score = m_playerManager->GetPlayer2Score();

    if (m_registry->HasComponent<TextComponent>(m_player1ScoreTextEntity))
    {
        auto &textComp = m_registry->GetComponent<TextComponent>(m_player1ScoreTextEntity);
        textComp.text = "Player 1 Score: " + std::to_string(player1Score);
    }

    if (m_registry->HasComponent<TextComponent>(m_player2ScoreTextEntity))
    {
        auto &textComp = m_registry->GetComponent<TextComponent>(m_player2ScoreTextEntity);
        textComp.text = "Player 2 Score: " + std::to_string(player2Score);
    }
}

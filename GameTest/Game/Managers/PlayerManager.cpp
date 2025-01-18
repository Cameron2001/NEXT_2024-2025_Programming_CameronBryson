#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager()
{
}

void PlayerManager::SetPlayer1(Entity player)
{
    m_player1 = player;
}

void PlayerManager::SetPlayer2(Entity player)
{
    m_player2 = player;
}

Entity PlayerManager::GetPlayer1()
{
    return m_player1;
}

Entity PlayerManager::GetPlayer2()
{
    return m_player2;
}

int PlayerManager::GetPlayer1Score() const
{
    return m_player1Score;
}

int PlayerManager::GetPlayer2Score() const
{
    return m_player2Score;
}

int PlayerManager::GetPlayer1TotalScore() const
{
    return m_player1TotalScore;
}

int PlayerManager::GetPlayer2TotalScore() const
{
    return m_player2TotalScore;
}

void PlayerManager::IncrementPlayer1Score()
{
    m_player1Score++;
}

void PlayerManager::IncrementPlayer2Score()
{
    m_player2Score++;
}

void PlayerManager::IncrementPlayer1TotalScore()
{
    m_player1TotalScore += m_player1Score;
}

void PlayerManager::IncrementPlayer2TotalScore()
{
    m_player2TotalScore += m_player2Score;
}

void PlayerManager::IncrementCurrentPlayerScore()
{
    if (m_player1Turn)
    {
        IncrementPlayer1Score();
    }
    else
    {
        IncrementPlayer2Score();
    }
}

void PlayerManager::IncrementCurrentPlayerTotalScore()
{
    if (m_player1Turn)
    {
        IncrementPlayer1TotalScore();
    }
    else
    {
        IncrementPlayer2TotalScore();
    }
}

void PlayerManager::ResetScores()
{
    m_player1Score = 0;
    m_player2Score = 0;
}

void PlayerManager::ResetTotalScores()
{
    m_player1Score = 0;
    m_player2Score = 0;
}

void PlayerManager::SwapTurn()
{
    m_player1Turn = !m_player1Turn;
}

bool PlayerManager::IsPlayer1Turn() const
{
    return m_player1Turn;
}

bool PlayerManager::IsPlayer1Complete() const
{
    return m_player1Complete;
}

bool PlayerManager::IsPlayer2Complete() const
{
    return m_player2Complete;
}

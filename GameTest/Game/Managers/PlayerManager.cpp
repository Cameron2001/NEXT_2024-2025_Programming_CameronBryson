// File: GameTest/Game/Managers/PlayerManager.cpp
#include "stdafx.h"
#include "PlayerManager.h"

PlayerManager::PlayerManager()
= default;

void PlayerManager::SetPlayer1(const Entity player)
{
    m_player1 = player;
}

void PlayerManager::SetPlayer2(const Entity player)
{
    m_player2 = player;
}

void PlayerManager::SetHole(const Entity hole)
{
    m_hole = hole;
}

Entity PlayerManager::GetPlayer1() const
{
    return m_player1;
}

Entity PlayerManager::GetPlayer2() const
{
    return m_player2;
}

Entity PlayerManager::GetHole() const
{
    return m_hole;
}

int PlayerManager::GetPlayer1Score() const
{
    return m_player1Score;
}

int PlayerManager::GetPlayer2Score() const
{
    return m_player2Score;
}

void PlayerManager::IncrementPlayer1Score()
{
    m_player1Score++;
}

void PlayerManager::IncrementPlayer2Score()
{
    m_player2Score++;
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


void PlayerManager::ResetScores()
{
    m_player1Score = 0;
    m_player2Score = 0;
    m_player1Complete = false;
    m_player2Complete = false;
    m_player1Turn = true;
}

void PlayerManager::SwapTurn()
{
    if (m_player1Complete && m_player2Complete)
    {
        // Both players have completed the course; no further turns.
        return;
    }

    if (m_player1Complete && !m_player2Complete)
    {
        m_player1Turn = false; // Only Player 2 remains active.
        return;
    }

    if (m_player2Complete && !m_player1Complete)
    {
        m_player1Turn = true; // Only Player 1 remains active.
        return;
    }

    // Both players are still active; swap turns normally.
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

void PlayerManager::SetPlayer1Complete(bool complete)
{
    m_player1Complete = complete;
}

void PlayerManager::SetPlayer2Complete(bool complete)
{
    m_player2Complete = complete;
}

#pragma once
#include "Game/Core/Entity.h"
class PlayerManager
{
  public:
    PlayerManager();
    ~PlayerManager() = default;
    void SetPlayer1(Entity player);
    void SetPlayer2(Entity player);
    Entity GetPlayer1();
    Entity GetPlayer2();
    int GetPlayer1Score() const;
    int GetPlayer2Score() const;
    int GetPlayer1TotalScore() const;
    int GetPlayer2TotalScore() const;
    void IncrementPlayer1Score();
    void IncrementPlayer2Score();
    void IncrementPlayer1TotalScore();
    void IncrementPlayer2TotalScore();
    void IncrementCurrentPlayerScore();
    void IncrementCurrentPlayerTotalScore();
    void ResetScores();
    void ResetTotalScores();
    void SwapTurn();
    bool IsPlayer1Turn() const;
    bool IsPlayer1Complete() const;
    bool IsPlayer2Complete() const;
  private:
    Entity m_player1;
    Entity m_player2;
    int m_player1Score = 0;
    int m_player2Score = 0;
    int m_player1TotalScore = 0;
    int m_player2TotalScore = 0;
    bool m_player1Turn = true;
    bool m_player1Complete = false;
    bool m_player2Complete = false;

};

#pragma once
#include "Game/Core/Entity.h"

class PlayerManager
{
  public:
    PlayerManager();
    ~PlayerManager() = default;

    void SetPlayer1(Entity player);
    void SetPlayer2(Entity player);
    void SetHole(Entity hole);

    Entity GetPlayer1() const;
    Entity GetPlayer2() const;
    Entity GetHole() const;

    int GetPlayer1Score() const;
    int GetPlayer2Score() const;

    void IncrementPlayer1Score();
    void IncrementPlayer2Score();

    void IncrementCurrentPlayerScore();

    void ResetScores();

    void SwapTurn();

    bool IsPlayer1Turn() const;
    bool IsPlayer1Complete() const;
    bool IsPlayer2Complete() const;

    void SetPlayer1Complete(const bool complete);
    void SetPlayer2Complete(const bool complete);

  private:
    Entity m_player1;
    Entity m_player2;
    Entity m_hole;

    int m_player1Score = 0;
    int m_player2Score = 0;

    bool m_player1Turn = true;
    bool m_player1Complete = false;
    bool m_player2Complete = false;
};

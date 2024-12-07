#pragma once
struct PlayerComponent
{
    PlayerComponent(float moveSpeed) : moveSpeed(moveSpeed)
    {
    }

    float moveSpeed = 0.0f;
};
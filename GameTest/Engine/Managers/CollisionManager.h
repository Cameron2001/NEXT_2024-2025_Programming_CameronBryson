#pragma once
#include "Engine/Core/Collision.h"
#include <Engine/Math/Vector3.h>
#include <unordered_map>
#include <utility>
class CollisionManager
{
  public:
    // void collisionDetected(unsigned short ID1, unsigned short ID2, float penetration, const FVector3 &normal);
    // void collisionNotDetected(unsigned short ID1, unsigned short ID2);

  private:
    // std::unordered_map<std::pair<unsigned short, unsigned short>, Collision, HashPair> m_collisions;
};

// Keep track of collisions

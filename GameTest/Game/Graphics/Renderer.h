#pragma once
#include "Game/Math/Vector2.h"
#include "Game/Math/Vector3.h"
#include <string>
#include <vector>
#include "Edge.h"
#include <utility>

class Renderer
{
  public:
    static void DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color);
    static void DrawLine(const FVector3 &start, const FVector3 &end, const FVector3 &color);
    static void DrawLine(const Edge2D &edge, const FVector3 &color);
    static void PrintText(const std::string &text, const FVector2 &position, const FVector3 &color);
    static void DrawParticle(const FVector2 &center, float length, float rotation, const FVector3 &color);

  private:
    static FVector3 ndcToScreen(const FVector3 &ndc);
    static FVector2 ndcToScreen(const FVector2 &ndc);
    static Edge2D LiangBarsky(const Edge2D &edge);
};

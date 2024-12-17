#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include <string>
#include <vector>

class Renderer2D
{
  public:
    static void DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color);
    static void DrawLine(const FVector3 &start, const FVector3 &end, const FVector3 &color);
    static void DrawPolygon(const std::vector<FVector2> &vertices, const FVector3 &color);
    static void DrawPolygon(const std::vector<FVector3> &vertices, const FVector3 &color);
    static void PrintText(const std::string &text, const FVector2 &position, const FVector3 &color);
    static void DrawCross(const FVector2 &center, float radius, const FVector3 &color);
    static void DrawCross(const FVector3 &center, float radius, const FVector3 &color);

  private:
    static FVector3 ndcToScreen(const FVector3 &ndc);
    static FVector2 ndcToScreen(const FVector2 &ndc);
};

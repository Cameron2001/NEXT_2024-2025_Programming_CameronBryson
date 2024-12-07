#pragma once
#include <string>
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"

class Renderer2D
{
public:
    static void DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color);
    static void PrintText(const std::string &text, const FVector2 &position, const FVector3 &color);
};
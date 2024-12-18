#include "stdafx.h"
#include "Renderer2D.h"
#include "App/app.h"
int screenWidth = 1280;
int screenHeight = 720;
void Renderer2D::DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color)
{
    FVector2 startScreen = ndcToScreen(start);
    FVector2 endScreen = ndcToScreen(end);
    App::DrawLine(startScreen.X, startScreen.Y, endScreen.X, endScreen.Y, color.X, color.Y, color.Z);
}

void Renderer2D::DrawLine(const FVector3 &start, const FVector3 &end, const FVector3 &color)
{
    FVector3 startScreen = ndcToScreen(start);
    FVector3 endScreen = ndcToScreen(end);
    App::DrawLine(startScreen.X, startScreen.Y, endScreen.X, endScreen.Y, color.X, color.Y, color.Z);
}

void Renderer2D::DrawPolygon(const std::vector<FVector2> &vertices, const FVector3 &color)
{
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        DrawLine(vertices[i], vertices[(i + 1) % vertices.size()], color);
    }
}

void Renderer2D::DrawPolygon(const std::vector<FVector3> &vertices, const FVector3 &color)
{
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        DrawLine(vertices[i], vertices[(i + 1) % vertices.size()], color);
    }
}

void Renderer2D::PrintText(const std::string &text, const FVector2 &position, const FVector3 &color)
{
    App::Print(position.X, position.Y, text.c_str(), color.X, color.Y, color.Z);
}

void Renderer2D::DrawCross(const FVector2 &center, const float radius, const FVector3 &color)
{
    DrawLine(center, FVector2(center.X + radius, center.Y), color);
    DrawLine(center, FVector2(center.X, center.Y + radius), color);
    DrawLine(center, FVector2(center.X - radius, center.Y), color);
    DrawLine(center, FVector2(center.X, center.Y - radius), color);
}

void Renderer2D::DrawCross(const FVector3 &center, const float radius, const FVector3 &color)
{
    DrawLine(center, FVector3(center.X + radius, center.Y, center.Z), color);
    DrawLine(center, FVector3(center.X, center.Y + radius, center.Z), color);
    DrawLine(center, FVector3(center.X - radius, center.Y, center.Z), color);
    DrawLine(center, FVector3(center.X, center.Y - radius, center.Z), color);
}

void Renderer2D::DrawParticle(const FVector2 &center, const float length, const float rotation, const FVector3 &color)
{
    // works for now but should be changed later
    float halfLength = length / 2.0f;
    float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    float offsetX = halfLength * cos(rotation);
    float offsetY = halfLength * sin(rotation) * aspectRatio;
    FVector2 start(center.X - offsetX, center.Y - offsetY);
    FVector2 end(center.X + offsetX, center.Y + offsetY);
    DrawLine(start, end, color);
}

FVector3 Renderer2D::ndcToScreen(const FVector3 &ndc)
{
    FVector3 screen;
    screen.X = (ndc.X + 1) * 0.5f * screenWidth;
    screen.Y = (ndc.Y + 1) * 0.5f * screenHeight;
    screen.Z = ndc.Z; // Z coordinate remains the same
    return screen;
}

FVector2 Renderer2D::ndcToScreen(const FVector2 &ndc)
{
    FVector2 screen;
    screen.X = (ndc.X + 1) * 0.5f * screenWidth;
    screen.Y = (ndc.Y + 1) * 0.5f * screenHeight;
    return screen;
}

#include "stdafx.h"
#include "App/app.h"
#include "Renderer2D.h"
#include <cmath>
#include <string>
#include <vector>
#include "Edge.h"
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <algorithm>
int screenWidth = 1280;
int screenHeight = 720;
constexpr float NDC = 1.00f;
constexpr float xNDCMax = NDC;
constexpr float xNDCMin = -NDC;
constexpr float yNDCMax = NDC;
constexpr float yNDCMin = -NDC;
void Renderer2D::DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color)
{
    const FVector2 startScreen = ndcToScreen(start);
    const FVector2 endScreen = ndcToScreen(end);
    App::DrawLine(startScreen.X, startScreen.Y, endScreen.X, endScreen.Y, color.X, color.Y, color.Z);
}

void Renderer2D::DrawLine(const FVector3 &start, const FVector3 &end, const FVector3 &color)
{
    const FVector3 startScreen = ndcToScreen(start);
    const FVector3 endScreen = ndcToScreen(end);
    App::DrawLine(startScreen.X, startScreen.Y, endScreen.X, endScreen.Y, color.X, color.Y, color.Z);
}
void Renderer2D::DrawLine(const Edge2D &edge, const FVector3 &color)
{
    auto clippedEdge = LiangBarsky(edge);
    if (clippedEdge.start != clippedEdge.end)
    {
        DrawLine(clippedEdge.start, clippedEdge.end, color);
    }
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
    const float halfLength = length / 2.0f;
    const float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    const float offsetX = halfLength * cos(rotation);
    const float offsetY = halfLength * sin(rotation) * aspectRatio;
    const FVector2 start(center.X - offsetX, center.Y - offsetY);
    const FVector2 end(center.X + offsetX, center.Y + offsetY);
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

Edge2D Renderer2D::LiangBarsky(const Edge2D &edge)
{
    constexpr float xMin = xNDCMin;
    constexpr float xMax = xNDCMax;
    constexpr float yMin = yNDCMin;
    constexpr float yMax = yNDCMax;

    float t0 = 0.0f, t1 = 1.0f;

    float p[4], q[4];

    const FVector2 d = edge.end - edge.start; // Direction vector

    p[0] = -d.X;
    q[0] = edge.start.X - xMin;
    p[1] = d.X;
    q[1] = xMax - edge.start.X;
    p[2] = -d.Y;
    q[2] = edge.start.Y - yMin;
    p[3] = d.Y;
    q[3] = yMax - edge.start.Y;

    for (int i = 0; i < 4; ++i)
    {
        if (p[i] == 0)
        {
            if (q[i] < 0)
            {
                // Line is parallel to plane and outside the boundary
                return {}; // Return an invalid edge
            }
            // Line is parallel to plane and inside; continue
        }
        else
        {
            float t = q[i] / p[i];
            if (p[i] < 0)
            {
                if (t > t1)
                    return {}; // Line is outside
                t0 = (std::max)(t, t0);
            }
            else
            {
                if (t < t0)
                    return {}; // Line is outside
                t1 = (std::min)(t, t1);
            }
        }
    }

    if (t0 > t1)
    {
        // Line is outside the clipping region
        return {}; // Return an invalid edge
    }

    FVector2 clippedStart = edge.start + d * t0;
    FVector2 clippedEnd = edge.start + d * t1;

    return {clippedStart, clippedEnd};
}

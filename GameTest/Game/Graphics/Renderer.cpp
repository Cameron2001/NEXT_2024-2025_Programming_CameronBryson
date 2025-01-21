#include "stdafx.h"
#include "App/app.h"
#include "Renderer.h"
#include <cmath>
#include <string>
#include <vector>
#include "Edge.h"
#include <Game/Math/Vector2.h>
#include <Game/Math/Vector3.h>
#include <algorithm>
int screenWidth = 1280;
int screenHeight = 720;
constexpr float NDC = 1.00f;
constexpr float xNDCMax = NDC;
constexpr float xNDCMin = -NDC;
constexpr float yNDCMax = NDC;
constexpr float yNDCMin = -NDC;
void Renderer::DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color)
{
    const FVector2 startScreen = ndcToScreen(start);
    const FVector2 endScreen = ndcToScreen(end);
    App::DrawLine(startScreen.x, startScreen.y, endScreen.x, endScreen.y, color.x, color.y, color.z);
}

void Renderer::DrawLine(const FVector3 &start, const FVector3 &end, const FVector3 &color)
{
    const FVector3 startScreen = ndcToScreen(start);
    const FVector3 endScreen = ndcToScreen(end);
    App::DrawLine(startScreen.x, startScreen.y, endScreen.x, endScreen.y, color.x, color.y, color.z);
}
void Renderer::DrawLine(const Edge2D &edge, const FVector3 &color)
{
    auto clippedEdge = LiangBarsky(edge);
    if (clippedEdge.start != clippedEdge.end)
    {
        DrawLine(clippedEdge.start, clippedEdge.end, color);
    }
}
void Renderer::PrintText(const std::string &text, const FVector2 &position, const FVector3 &color)
{
    App::Print(position.x, position.y, text.c_str(), color.x, color.y, color.z);
}
void Renderer::DrawParticle(const FVector2 &center, const float length, const float rotation, const FVector3 &color)
{
    // works for now but should be changed later
    const float halfLength = length / 2.0f;
    const float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    const float offsetX = halfLength * cos(rotation);
    const float offsetY = halfLength * sin(rotation) * aspectRatio;
    const FVector2 start(center.x - offsetX, center.y - offsetY);
    const FVector2 end(center.x + offsetX, center.y + offsetY);
    DrawLine(start, end, color);
}

FVector3 Renderer::ndcToScreen(const FVector3 &ndc)
{
    FVector3 screen;
    screen.x = (ndc.x + 1) * 0.5f * screenWidth;
    screen.y = (ndc.y + 1) * 0.5f * screenHeight;
    screen.z = ndc.z; // Z coordinate remains the same
    return screen;
}

FVector2 Renderer::ndcToScreen(const FVector2 &ndc)
{
    FVector2 screen;
    screen.x = (ndc.x + 1) * 0.5f * screenWidth;
    screen.y = (ndc.y + 1) * 0.5f * screenHeight;
    return screen;
}

Edge2D Renderer::LiangBarsky(const Edge2D &edge)
{
    constexpr float xMin = xNDCMin;
    constexpr float xMax = xNDCMax;
    constexpr float yMin = yNDCMin;
    constexpr float yMax = yNDCMax;

    float t0 = 0.0f, t1 = 1.0f;

    float p[4], q[4];

    const FVector2 d = edge.end - edge.start; // Direction vector

    p[0] = -d.x;
    q[0] = edge.start.x - xMin;
    p[1] = d.x;
    q[1] = xMax - edge.start.x;
    p[2] = -d.y;
    q[2] = edge.start.y - yMin;
    p[3] = d.y;
    q[3] = yMax - edge.start.y;

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

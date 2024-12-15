#include "stdafx.h"
#include "Renderer2D.h"
#include "App/app.h"

void Renderer2D::DrawLine(const FVector2 &start, const FVector2 &end, const FVector3 &color)
{
    App::DrawLine(start.X, start.Y, end.X, end.Y, color.X, color.Y, color.Z);
}

void Renderer2D::DrawLine(const FVector3 &start, const FVector3 &end, const FVector3 &color)
{
    App::DrawLine(start.X, start.Y, end.X, end.Y, color.X, color.Y, color.Z);
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
}

void Renderer2D::DrawCircle(const FVector3 &center, float radius, const FVector3 &color)
{
    App::DrawLine(center.X, center.Y, center.X + radius, center.Y, color.X, color.Y, color.Z);
    App::DrawLine(center.X, center.Y, center.X, center.Y + radius, color.X, color.Y, color.Z);
    App::DrawLine(center.X, center.Y, center.X - radius, center.Y, color.X, color.Y, color.Z);
    App::DrawLine(center.X, center.Y, center.X, center.Y - radius, color.X, color.Y, color.Z);
}

#include "stdafx.h"
#include "Edge.h"
#include "HiddenLineRemoval.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Triangle.h"
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Vector3.h>
#include <utility>
#include <vector>

std::vector<Triangle2D> Renderer::m_triangles;

void Renderer::QueueMesh(const Mesh &mesh, const Matrix4 &MVP)
{

    for (const auto &face : mesh.triangles)
    {

        // Transformed vertices
        FVector3 mvpVertex0 = MVP.TransformWithPerspectiveDivide(face.v0);
        FVector3 mvpVertex1 = MVP.TransformWithPerspectiveDivide(face.v1);
        FVector3 mvpVertex2 = MVP.TransformWithPerspectiveDivide(face.v2);
        if (QuickReject(Edge3D(mvpVertex0, mvpVertex1), Edge3D(mvpVertex1, mvpVertex2), Edge3D(mvpVertex2, mvpVertex0)))
        {
            continue;
        }
        const float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                                  ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant < 0)
        {
            continue;
        }
        float avgZ = (mvpVertex0.Z + mvpVertex1.Z + mvpVertex2.Z) / 3.0f;
        FVector2 v0(mvpVertex0.X, mvpVertex0.Y);
        FVector2 v1(mvpVertex1.X, mvpVertex1.Y);
        FVector2 v2(mvpVertex2.X, mvpVertex2.Y);
        m_triangles.emplace_back(v0, v1, v2, avgZ);
    }
}

void Renderer::QueueModel(const Model &model, const Matrix4 &MVP)
{
    for (const auto &mesh : model.meshes)
    {
        QueueMesh(mesh, MVP);
    }
}

void Renderer::SubmitQueue()
{
    if (m_triangles.empty())
        return;
    const HiddenLineRemoval hlr(m_triangles);
    const std::vector<Edge2D> visibleSegments = hlr.removeHiddenLines();
    for (const auto &edge : visibleSegments)
    {
        Edge2D clippedEdge = LiangBarsky(edge);
        if (clippedEdge.start != clippedEdge.end)
        {

            Renderer2D::DrawLine(clippedEdge.start, clippedEdge.end, {1.0f, 1.0f, 1.0f});
        }
    }
    ClearQueue();
}

void Renderer::ClearQueue()
{
    m_triangles.clear();
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
                t0 = std::max(t, t0);
            }
            else
            {
                if (t < t0)
                    return {}; // Line is outside
                t1 = std::min(t, t1);
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

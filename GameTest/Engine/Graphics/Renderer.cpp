#include "stdafx.h"
#include "Edge.h"
#include "Face.h"
#include "HiddenLineRemoval.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include <algorithm>
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Vector3.h>
#include <unordered_set>
#include <vector>

std::vector<Face> Renderer::m_triangles;
void Renderer::QueueMesh(const Mesh &mesh, const Matrix4 &MVP)
{
    for (const auto &face : mesh.faces)
    {

        // Transformed vertices
        FVector3 mvpVertex0 = MVP.TransformWithPerspectiveDivide(face.v0);
        FVector3 mvpVertex1 = MVP.TransformWithPerspectiveDivide(face.v1);
        FVector3 mvpVertex2 = MVP.TransformWithPerspectiveDivide(face.v2);
        if (QuickReject(Edge3D(mvpVertex0, mvpVertex1), Edge3D(mvpVertex1, mvpVertex2), Edge3D(mvpVertex2, mvpVertex0)))
        {
            continue;
        }
        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                            ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant < 0)
        {
            continue;
        }
        m_triangles.emplace_back(mvpVertex0, mvpVertex1, mvpVertex2);
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
    //for (auto &triangle : m_triangles)
    //{
    //    Renderer2D::DrawPolygon({triangle.v0, triangle.v1, triangle.v2}, {1.0f, 1.0f, 1.0f});
    //}
    HiddenLineRemoval hlr(m_triangles);
    std::vector<Edge3D> visibleSegments = hlr.removeHiddenLines();

    // Use an unordered_set with the custom hash to filter out duplicate edges
    std::unordered_set<Edge3D, Edge3DHash> uniqueVisibleEdges;
    uniqueVisibleEdges.reserve(visibleSegments.size());
    uniqueVisibleEdges.insert(visibleSegments.begin(), visibleSegments.end());
    int screenWidth = 1280;  
    int screenHeight = 720; 
    for (const auto &edge : uniqueVisibleEdges)
    {
        Edge3D clippedEdge = LiangBarsky(edge);
        // Check if the clipped edge is valid before drawing
        if (clippedEdge.start != clippedEdge.end)
        {
            FVector3 startScreen;
            startScreen.X = (clippedEdge.start.X + 1) * 0.5f * screenWidth;
            startScreen.Y = (clippedEdge.start.Y + 1) * 0.5f * screenHeight;
            startScreen.Z = clippedEdge.start.Z; // Z coordinate remains the same

            FVector3 endScreen;
            endScreen.X = (clippedEdge.end.X + 1) * 0.5f * screenWidth;
            endScreen.Y = (clippedEdge.end.Y + 1) * 0.5f * screenHeight;
            endScreen.Z = clippedEdge.end.Z; // Z coordinate remains the same

            Renderer2D::DrawLine(startScreen, endScreen, {1.0f, 1.0f, 1.0f});
        }
    }
    ClearQueue();
}



void Renderer::ClearQueue()
{
    m_triangles.clear();
}




Edge3D Renderer::LiangBarsky(const Edge3D &edge)
{
    float xMin = xNDCMin, xMax = xNDCMax;
    float yMin = yNDCMin, yMax = yNDCMax;

    float t0 = 0.0f, t1 = 1.0f;

    float p[4], q[4];

    FVector3 d = edge.end - edge.start; // Direction vector

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
                return Edge3D(); // Return an invalid edge
            }
            // Line is parallel to plane and inside; continue
        }
        else
        {
            float t = q[i] / p[i];
            if (p[i] < 0)
            {
                if (t > t1)
                    return Edge3D(); // Line is outside
                if (t > t0)
                    t0 = t;
            }
            else
            {
                if (t < t0)
                    return Edge3D(); // Line is outside
                if (t < t1)
                    t1 = t;
            }
        }
    }

    if (t0 > t1)
    {
        // Line is outside the clipping region
        return Edge3D(); // Return an invalid edge
    }

    FVector3 clippedStart = edge.start + d * t0;
    FVector3 clippedEnd = edge.start + d * t1;

    return Edge3D(clippedStart, clippedEnd);
}
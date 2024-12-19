#include "stdafx.h"
#include "Edge.h"
#include "Triangle.h"
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

std::vector<const Triangle *> Renderer::m_triangles;
std::unordered_map<Edge3D, std::vector<size_t>, Edge3DHash> Renderer::m_edgeToTriangleMap;

void Renderer::QueueMesh(const Mesh &mesh, const Matrix4 &MVP)
{
    m_edgeToTriangleMap.reserve(m_edgeToTriangleMap.size() + mesh.triangles.size() * 3);

    for (auto &triangle : mesh.triangles)
    {
        // Transform vertices
        FVector3 mvpVertex0 = MVP.TransformWithPerspectiveDivide(triangle.v0);
        FVector3 mvpVertex1 = MVP.TransformWithPerspectiveDivide(triangle.v1);
        FVector3 mvpVertex2 = MVP.TransformWithPerspectiveDivide(triangle.v2);

        if (QuickReject(Edge3D(mvpVertex0, mvpVertex1), Edge3D(mvpVertex1, mvpVertex2), Edge3D(mvpVertex2, mvpVertex0)))
        {
            continue;
        }

        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                            ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        bool isFrontFacing = determinant > 0;

        triangle.isFrontFacing = isFrontFacing;

        m_triangles.emplace_back(&triangle);

        size_t triIndex = m_triangles.size() - 1;

        Edge3D edges[3] = {Edge3D(mvpVertex0, mvpVertex1), Edge3D(mvpVertex1, mvpVertex2),
                           Edge3D(mvpVertex2, mvpVertex0)};

        for (const auto &edge : edges)
        {
            m_edgeToTriangleMap[edge].push_back(triIndex);
        }
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

    std::vector<Edge3D> silhouetteEdges;

    // Identify silhouette edges
    for (const auto &pair : m_edgeToTriangleMap)
    {
        const Edge3D &edge = pair.first;
        const std::vector<size_t> &adjacentTriangles = pair.second;

        bool isSilhouette = false;

        if (adjacentTriangles.size() == 1)
        {
            // Edge is on the boundary of the mesh
            size_t triIndex = adjacentTriangles[0];
            if (m_triangles[triIndex]->isFrontFacing)
            {
                isSilhouette = true;
            }
        }
        else if (adjacentTriangles.size() == 2)
        {
            size_t triIndex0 = adjacentTriangles[0];
            size_t triIndex1 = adjacentTriangles[1];

            bool facing0 = m_triangles[triIndex0]->isFrontFacing;
            bool facing1 = m_triangles[triIndex1]->isFrontFacing;

            if (facing0 != facing1)
            {
                isSilhouette = true;
            }
        }
        else
        {
            bool facingFirst = m_triangles[adjacentTriangles[0]]->isFrontFacing;
            for (size_t i = 1; i < adjacentTriangles.size(); ++i)
            {
                if (m_triangles[adjacentTriangles[i]]->isFrontFacing != facingFirst)
                {
                    isSilhouette = true;
                    break;
                }
            }
        }

        if (isSilhouette)
        {
            silhouetteEdges.push_back(edge);
        }
    }

    for (const auto &edge : silhouetteEdges)
    {
        Edge3D clippedEdge = LiangBarsky(edge);
        if (clippedEdge.start != clippedEdge.end)
        {
            Renderer2D::DrawLine(clippedEdge.start, clippedEdge.end, {1.0f, 1.0f, 1.0f});
        }
    }

    ClearQueue();

    // for (auto &triangle : m_triangles)
    //{
    //     Renderer2D::DrawPolygon({triangle.v0, triangle.v1, triangle.v2}, {1.0f, 1.0f, 1.0f});
    // }
    // HiddenLineRemoval hlr(m_triangles);
    // std::vector<Edge3D> visibleSegments = hlr.removeHiddenLines();
    //// Use an unordered_set with the custom hash to filter out duplicate edges
    // std::unordered_set<Edge3D, Edge3DHash> uniqueVisibleEdges;
    // uniqueVisibleEdges.reserve(visibleSegments.size());
    // uniqueVisibleEdges.insert(visibleSegments.begin(), visibleSegments.end());
    // for (const auto &edge : uniqueVisibleEdges)
    //{
    //     Edge3D clippedEdge = LiangBarsky(edge);
    //     // Check if the clipped edge is valid before drawing
    //     if (clippedEdge.start != clippedEdge.end)
    //     {

    //        Renderer2D::DrawLine(clippedEdge.start, clippedEdge.end, {1.0f, 1.0f, 1.0f});
    //    }
    //}
    ClearQueue();
}

void Renderer::ClearQueue()
{
    m_triangles.clear();
    m_edgeToTriangleMap.clear();
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

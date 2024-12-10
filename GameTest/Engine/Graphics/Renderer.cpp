#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include <algorithm>
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Math/Vector4.h>
#include <cmath>
#include <limits>
#include <unordered_set>
#include <utility>
#include <array>
#include <vector>
#include <algorithm>
using Segment = std::pair<FVector2, FVector2>;
using FaceList = std::vector<RenderFace>;
using PointList = std::vector<FVector2>;
const FVector3 VIEW_DIRECTION(0.0f, 0.0f, 1.0f);
const float NDC = 0.50f;
const float xNDCMax = NDC;
const float xNDCMin = -NDC;
const float yNDCMax = NDC;
const float yNDCMin = -NDC;
const float EPSILON = 0.001f;
std::vector<RenderFace> Renderer::QueuedFaces;

void Renderer::QueueMesh(const Mesh &mesh, const Matrix4 &MVP)
{
    for (const auto &face : mesh.faces)
    {
        // Transformed vertices
        FVector3 mvpVertex0 = MVP.TransformWithPerspectiveDivide(face.v0);
        FVector3 mvpVertex1 = MVP.TransformWithPerspectiveDivide(face.v1);
        FVector3 mvpVertex2 = MVP.TransformWithPerspectiveDivide(face.v2);

        // Within ndc space
        if (!IsOnScreen(mvpVertex0) && !IsOnScreen(mvpVertex1) && !IsOnScreen(mvpVertex2))
        {
            continue;
        }
        // Backface culling
        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                            ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant < EPSILON)
        {
            continue;
        }
        float minDepth = std::min({mvpVertex0.Z, mvpVertex1.Z, mvpVertex2.Z});
        QueuedFaces.emplace_back(FVector2{mvpVertex0.X, mvpVertex0.Y}, FVector2{mvpVertex1.X, mvpVertex1.Y},
                                  FVector2{mvpVertex2.X, mvpVertex2.Y}, minDepth);
        //QueuedFaces.emplace_back(mvpVertex0, mvpVertex1, mvpVertex2,viewNormal);
    }
}

void Renderer::QueueModel(const Model &model, const Matrix4 &MVP)
{
    for (const auto &mesh : model.meshes)
    {
        QueueMesh(mesh, MVP);
    }
}

bool Renderer::IsOnScreen(const FVector3 &point)
{
    return (point.X >= xNDCMin - EPSILON && point.X <= xNDCMax + EPSILON) &&
           (point.Y >= yNDCMin - EPSILON && point.Y <= yNDCMax + EPSILON);
}


bool Renderer::IsInside(const FVector2 &point, ClipEdge edge)
{
    switch (edge)
    {
    case LEFT:
        return point.X >= xNDCMin - EPSILON;
    case RIGHT:
        return point.X <= xNDCMax + EPSILON;
    case BOTTOM:
        return point.Y >= yNDCMin - EPSILON;
    case TOP:
        return point.Y <= yNDCMax + EPSILON;
    }
    return false;
}

std::vector<FVector2> Renderer::ClipPolygon(const std::vector<FVector2> &subjectPolygon)
{
    std::vector<FVector2> outputList = subjectPolygon;
    const std::vector<ClipEdge> clipEdges = {LEFT, RIGHT, BOTTOM, TOP};

    for (const auto &edge : clipEdges)
    {
        outputList = ClipAgainstEdge(outputList, edge);

        if (outputList.empty())
            break;
    }

    return outputList;
}

std::vector<FVector2> Renderer::ClipAgainstEdge(const std::vector<FVector2> &inputList, ClipEdge edge)
{
    std::vector<FVector2> outputList;

    for (size_t i = 0; i < inputList.size(); ++i)
    {
        const FVector2 &current_point = inputList[i];
        const FVector2 &prev_point = inputList[(i + inputList.size() - 1) % inputList.size()];

        bool current_inside = IsInside(current_point, edge);
        bool prev_inside = IsInside(prev_point, edge);

        FVector2 intersect;

        if (current_inside)
        {
            if (!prev_inside && ComputeIntersection(prev_point, current_point, edge, intersect))
            {
                outputList.push_back(intersect);
            }
            outputList.push_back(current_point);
        }
        else if (prev_inside && ComputeIntersection(prev_point, current_point, edge, intersect))
        {
            outputList.push_back(intersect);
        }
    }

    return outputList;
}


bool Renderer::ComputeIntersection(const FVector2 &p1, const FVector2 &p2, ClipEdge edge, FVector2 &intersect)
{
    float t = 0.0f;
    float denominator = 0.0f;

    switch (edge)
    {
    case LEFT:
        denominator = p2.X - p1.X;
        if (std::abs(denominator) < EPSILON)
            return false;
        t = (xNDCMin - p1.X) / denominator;
        break;
    case RIGHT:
        denominator = p2.X - p1.X;
        if (std::abs(denominator) < EPSILON)
            return false;
        t = (xNDCMax - p1.X) / denominator;
        break;
    case BOTTOM:
        denominator = p2.Y - p1.Y;
        if (std::abs(denominator) < EPSILON)
            return false;
        t = (yNDCMin - p1.Y) / denominator;
        break;
    case TOP:
        denominator = p2.Y - p1.Y;
        if (std::abs(denominator) < EPSILON)
            return false;
        t = (yNDCMax - p1.Y) / denominator;
        break;
    default:
        return false;
    }

    if (t < 0.0f || t > 1.0f)
        return false;

    intersect = p1 + (p2 - p1) * t;
    return true;
}




std::vector<float> Renderer::GetOcclusionPoints(const FVector2 &start, const FVector2 &end,
                                                const std::vector<RenderFace> &occluders)
{
    std::unordered_set<float> occlusionTs;

    const float x1 = start.X;
    const float y1 = start.Y;
    const float x2 = end.X;
    const float y2 = end.Y;

    for (const auto &occluder : occluders)
    {
        const FVector2 &o0 = occluder.v0;
        const FVector2 &o1 = occluder.v1;
        const FVector2 &o2 = occluder.v2;

       std::array<std::pair<FVector2, FVector2>, 3> occluderEdges = {{{o0, o1}, {o1, o2}, {o2, o0}}};

        for (const auto &edge : occluderEdges)
        {
            const FVector2 &p3 = edge.first;
            const FVector2 &p4 = edge.second;

            const float x3 = p3.X;
            const float y3 = p3.Y;
            const float x4 = p4.X;
            const float y4 = p4.Y;

            float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            if (std::abs(denom) < EPSILON)
                continue;

            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
            float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denom;

            if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
            {
                occlusionTs.insert(t);
            }
        }
    }

    std::vector<float> occlusionTsVector(occlusionTs.begin(), occlusionTs.end());
    std::sort(occlusionTsVector.begin(), occlusionTsVector.end());

    return occlusionTsVector;
}

bool Renderer::PointInTriangle(const FVector2 &pt, const FVector2 &v0, const FVector2 &v1, const FVector2 &v2)
{
    FVector2 v0v1 = v1 - v0;
    FVector2 v0v2 = v2 - v0;
    FVector2 v0pt = pt - v0;

    // Compute dot products
    float dot00 = v0v2.Dot(v0v2);
    float dot01 = v0v2.Dot(v0v1);
    float dot02 = v0v2.Dot(v0pt);
    float dot11 = v0v1.Dot(v0v1);
    float dot12 = v0v1.Dot(v0pt);

    // Compute barycentric coordinates
    float denom = dot00 * dot11 - dot01 * dot01;
    if (std::abs(denom) < EPSILON)
        return false;

    float invDenom = 1.0f / denom;
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}



std::vector<Segment> Renderer::GetVisibleSegments(const FVector2 &start, const FVector2 &end, const FaceList &occluders)
{
    std::vector<Segment> visibleSegments;

    std::vector<float> occlusionTs = GetOcclusionPoints(start, end, occluders);

    if (occlusionTs.empty())
    {
        visibleSegments.emplace_back(start, end);
        return visibleSegments;
    }

    occlusionTs.insert(occlusionTs.begin(), 0.0f);
    occlusionTs.push_back(1.0f);

    // Remove duplicates and sort (already sorted from GetOcclusionPoints)
    occlusionTs.erase(
        std::unique(occlusionTs.begin(), occlusionTs.end(), [](float a, float b) { return std::abs(a - b) < EPSILON; }),
        occlusionTs.end());

    for (size_t i = 0; i < occlusionTs.size() - 1; ++i)
    {
        float tStart = occlusionTs[i];
        float tEnd = occlusionTs[i + 1];

        float tMid = (tStart + tEnd) * 0.5f;
        FVector2 midpoint = start + (end - start) * tMid ;

        bool isOccluded = false;
        for (const auto &occluder : occluders)
        {
            if (PointInTriangle(midpoint, occluder.v0, occluder.v1, occluder.v2))
            {
                isOccluded = true;
                break;
            }
        }

        if (!isOccluded)
        {
            FVector2 segmentStart = start + (end - start) * tStart;
            FVector2 segmentEnd = start + (end - start) * tEnd;
            visibleSegments.emplace_back(segmentStart, segmentEnd);
        }
    }

    return visibleSegments;
}






void Renderer::SubmitQueue()
{
    if (QueuedFaces.empty())
        return;


    if (!std::is_sorted(QueuedFaces.begin(), QueuedFaces.end(), [](const RenderFace &a, const RenderFace &b) -> bool {
            return a.minDepth < b.minDepth;
        }))
    {
        std::sort(QueuedFaces.begin(), QueuedFaces.end(),
                  [](const RenderFace &a, const RenderFace &b) -> bool { return a.minDepth < b.minDepth; });
    }

    std::vector<RenderFace> occluders; 

    for (const auto &renderFace : QueuedFaces)
    {
        const FVector2 *polygon[3] = {&renderFace.v0, &renderFace.v1, &renderFace.v2};

        for (size_t j = 0; j < 3; ++j)
        {
            const FVector2 &start = *polygon[j];
            const FVector2 &end = *polygon[(j + 1) % 3];

            // Get visible segments after occlusion
            std::vector<std::pair<FVector2, FVector2>> visibleSegments = GetVisibleSegments(start, end, occluders);

            // Render each visible segment
            for (const auto &segment : visibleSegments)
            {
                Renderer2D::DrawLine({segment.first.X, segment.first.Y}, {segment.second.X, segment.second.Y}, {1, 1, 1}
                                     // Color: White
                );
            }
        }

        // After rendering, add the current face to occluders for future occlusion checks
        occluders.emplace_back(renderFace);
    }

    ClearQueue();
}

void Renderer::ClearQueue()
{
    QueuedFaces.clear();
}

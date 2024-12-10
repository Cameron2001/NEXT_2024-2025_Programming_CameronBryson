#include "stdafx.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include <Engine/Math/Matrix4.h>
#include <Engine/Math/Vector3.h>
#include <Engine/Math/Vector4.h>
#include <limits>
#include "Camera.h"
#include <algorithm>
const FVector3 VIEW_DIRECTION(0.0f, 0.0f, 1.0f);
const float NDC = 0.75f;
const float xNDCMax = NDC;
const float xNDCMin = -NDC;
const float yNDCMax = NDC;
const float yNDCMin = -NDC;
const float EPSILON = 0.001f;
std::vector<RenderFace> Renderer::QueuedFaces;

void Renderer::QueueMesh(const Mesh &mesh, const Matrix4 &MVP, const Matrix4& normalMatrix )
{
    for (const auto &face : mesh.faces)
    {
        //Transformed vertices
        FVector3 mvpVertex0 = MVP.TransformWithPerspectiveDivide(face.v0);
        FVector3 mvpVertex1 = MVP.TransformWithPerspectiveDivide(face.v1);
        FVector3 mvpVertex2 = MVP.TransformWithPerspectiveDivide(face.v2);
        

        // Within ndc space
        if (!IsOnScreen(mvpVertex0) && !IsOnScreen(mvpVertex1) && !IsOnScreen(mvpVertex2))
        {
            continue;
        }
        //Backface culling
        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) - (
                                (mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant < EPSILON)
        {
            continue;
        }
        float avgDepth = (mvpVertex0.Z + mvpVertex1.Z + mvpVertex2.Z) / 3.0f;
        Face newFace = {mvpVertex0, mvpVertex1, mvpVertex2};
        QueuedFaces.emplace_back(newFace, avgDepth);
        //QueuedFaces.emplace_back(mvpVertex0, mvpVertex1, mvpVertex2,viewNormal);
    }
}

void Renderer::QueueModel(const Model &model, const Matrix4 &MVP, const Matrix4& normalMatrix)
{
    for (const auto &mesh : model.meshes)
    {
        QueueMesh(mesh, MVP, normalMatrix);
    }
}

bool Renderer::IsOnScreen(const FVector3 &point)
{
    return (point.X >= xNDCMin && point.X <= xNDCMax ) &&
           (point.Y >= yNDCMin && point.Y <= yNDCMax );
}

std::vector<FVector3> Renderer::ClipPolygon(const std::vector<FVector3> &subjectPolygon,
                                            const std::vector<std::vector<FVector3>> &occluders)
{
    std::vector<FVector3> outputList = subjectPolygon;
    outputList.reserve(subjectPolygon.size());
    const std::vector<ClipEdge> clipEdges = {LEFT, RIGHT, BOTTOM, TOP};

    for (const auto &edge : clipEdges)
    {
        std::vector<FVector3> inputList = outputList;
        outputList.clear();
        outputList.reserve(inputList.size());

        if (inputList.empty())
            break;

        for (size_t i = 0; i < inputList.size(); ++i)
        {
            const FVector3 &current = inputList[i];
            const FVector3 &prev = inputList[(i + inputList.size() - 1) % inputList.size()];

            bool currentInside;
            bool prevInside;

            switch (edge)
            {
            case LEFT:
                currentInside = current.X >= xNDCMin;
                prevInside = prev.X >= xNDCMin;
                break;
            case RIGHT:
                currentInside = current.X <= xNDCMax;
                prevInside = prev.X <= xNDCMax;
                break;
            case BOTTOM:
                currentInside = current.Y >= yNDCMin;
                prevInside = prev.Y >= yNDCMin;
                break;
            case TOP:
                currentInside = current.Y <= yNDCMax;
                prevInside = prev.Y <= yNDCMax;
                break;
            }

            FVector3 intersect;
            bool hasIntersection = ComputeIntersection(prev, current, edge, intersect);

            if (currentInside)
            {
                if (!prevInside && hasIntersection)
                {
                    outputList.push_back(intersect);
                }
                outputList.push_back(current);
            }
            else if (prevInside && hasIntersection)
            {
                outputList.push_back(intersect);
            }
        }
    }

    return outputList;
}


bool Renderer::ComputeIntersection(const FVector3 &p1, const FVector3 &p2, ClipEdge edge, FVector3 &intersect)
{
    const float dx1 = p2.X - p1.X;
    const float dy1 = p2.Y - p1.Y;

    switch (edge)
    {
    case LEFT:
        if (dx1 == 0.0f)
            return false;
        intersect.X = xNDCMin;
        intersect.Y = p1.Y + dy1 * (xNDCMin - p1.X) / dx1;
        break;
    case RIGHT:
        if (dx1 == 0.0f)
            return false;
        intersect.X = xNDCMax;
        intersect.Y = p1.Y + dy1 * (xNDCMax - p1.X) / dx1;
        break;
    case BOTTOM:
        if (dy1 == 0.0f)
            return false;
        intersect.Y = yNDCMin;
        intersect.X = p1.X + dx1 * (yNDCMin - p1.Y) / dy1;
        break;
    case TOP:
        if (dy1 == 0.0f)
            return false;
        intersect.Y = yNDCMax;
        intersect.X = p1.X + dx1 * (yNDCMax - p1.Y) / dy1;
        break;
    }

    intersect.Z = 0.0f;
    return true;
}


std::vector<float> Renderer::GetOcclusionPoints(const FVector3 &start, const FVector3 &end,
                                                const std::vector<RenderFace> &occluders)
{
    std::vector<float> occlusionTs;

    const float x1 = start.X;
    const float y1 = start.Y;
    const float x2 = end.X;
    const float y2 = end.Y;

    for (const auto &occluder : occluders)
    {
        const FVector3 &o0 = occluder.face.v0;
        const FVector3 &o1 = occluder.face.v1;
        const FVector3 &o2 = occluder.face.v2;

        std::vector<std::pair<FVector3, FVector3>> occluderEdges = {{o0, o1}, {o1, o2}, {o2, o0}};

        for (const auto &edge : occluderEdges)
        {
            const FVector3 &p3 = edge.first;
            const FVector3 &p4 = edge.second;

            const float x3 = p3.X;
            const float y3 = p3.Y;
            const float x4 = p4.X;
            const float y4 = p4.Y;

            float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            if (denom == 0.0f)
                continue; 

            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denom;
            float u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) / denom;

            if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
            {
                occlusionTs.push_back(t);
            }
        }
    }


    std::sort(occlusionTs.begin(), occlusionTs.end());
    occlusionTs.erase(
        std::unique(occlusionTs.begin(), occlusionTs.end(), [](float a, float b) { return std::abs(a - b) < EPSILON; }),
        occlusionTs.end());

    return occlusionTs;
}

std::vector<std::pair<FVector3, FVector3>> Renderer::GetVisibleSegments(const FVector3 &start, const FVector3 &end,
                                                                        const std::vector<RenderFace> &occluders)
{
    std::vector<std::pair<FVector3, FVector3>> visibleSegments;


    std::vector<float> occlusionTs = GetOcclusionPoints(start, end, occluders);


    if (occlusionTs.empty())
    {
        visibleSegments.emplace_back(start, end);
        return visibleSegments;
    }


    occlusionTs.insert(occlusionTs.begin(), 0.0f);
    occlusionTs.push_back(1.0f);


    std::sort(occlusionTs.begin(), occlusionTs.end());

    size_t uniqueIdx = 0;
    for (size_t i = 1; i < occlusionTs.size(); ++i)
    {
        if ((occlusionTs[i] - occlusionTs[uniqueIdx]) * (occlusionTs[i] - occlusionTs[uniqueIdx]) >= EPSILON * EPSILON)
        {
            ++uniqueIdx;
            occlusionTs[uniqueIdx] = occlusionTs[i];
        }
    }
    occlusionTs.resize(uniqueIdx + 1);

    for (size_t i = 0; i < occlusionTs.size() - 1; ++i)
    {
        float tStart = occlusionTs[i];
        float tEnd = occlusionTs[i + 1];

        float tMid = (tStart + tEnd) / 2.0f;
        FVector3 midpoint(start.X + tMid * (end.X - start.X), start.Y + tMid * (end.Y - start.Y), 0.0f);

        bool isOccluded = false;
        for (const auto &occluder : occluders)
        {
            const FVector3 &v0 = occluder.face.v0;
            const FVector3 &v1 = occluder.face.v1;
            const FVector3 &v2 = occluder.face.v2;


            float denom = ((v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y));
            if (denom == 0.0f)
                continue; 

            float a = ((v1.Y - v2.Y) * (midpoint.X - v2.X) + (v2.X - v1.X) * (midpoint.Y - v2.Y)) / denom;
            float b = ((v2.Y - v0.Y) * (midpoint.X - v2.X) + (v0.X - v2.X) * (midpoint.Y - v2.Y)) / denom;
            float c = 1.0f - a - b;

            if (a >= 0.0f && b >= 0.0f && c >= 0.0f)
            {
                isOccluded = true;
                break; 
            }
        }

        if (isOccluded)
        {
            continue;
        }

        // Add the visible segment
        FVector3 segmentStart(start.X + tStart * (end.X - start.X), start.Y + tStart * (end.Y - start.Y), 0.0f);
        FVector3 segmentEnd(start.X + tEnd * (end.X - start.X), start.Y + tEnd * (end.Y - start.Y), 0.0f);
        visibleSegments.emplace_back(segmentStart, segmentEnd);
    }

    return visibleSegments;
}





void Renderer::SubmitQueue()
{
    if (QueuedFaces.empty())
        return;


    if (!std::is_sorted(QueuedFaces.begin(), QueuedFaces.end(), [](const RenderFace &a, const RenderFace &b) -> bool {
            return a.averageDepth < b.averageDepth;
        }))
    {
        std::sort(QueuedFaces.begin(), QueuedFaces.end(),
                  [](const RenderFace &a, const RenderFace &b) -> bool { return a.averageDepth < b.averageDepth; });
    }

    std::vector<RenderFace> occluders; 

    for (const auto &renderFace : QueuedFaces)
    {
        const FVector3 *polygon[3] = {&renderFace.face.v0, &renderFace.face.v1, &renderFace.face.v2};

        for (size_t j = 0; j < 3; ++j)
        {
            const FVector3 &start = *polygon[j];
            const FVector3 &end = *polygon[(j + 1) % 3];

            // Get visible segments after occlusion
            std::vector<std::pair<FVector3, FVector3>> visibleSegments = GetVisibleSegments(start, end, occluders);

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

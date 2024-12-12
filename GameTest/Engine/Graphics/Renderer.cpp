#include "stdafx.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
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
#include "Engine/Graphics/HiddenLine.h"
const FVector3 VIEW_DIRECTION(0.0f, 0.0f, 1.0f);
const float NDC = 1.00f;
const float xNDCMax = NDC;
const float xNDCMin = -NDC;
const float yNDCMax = NDC;
const float yNDCMin = -NDC;
const float EPSILON = 0.0001f;
std::vector<Edge> Renderer::RenderQueue;

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
            //continue;
        }
        // Backface culling
        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) -
                            ((mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant < EPSILON)
        {
            continue;
        }
        float minDepth = std::min({mvpVertex0.Z, mvpVertex1.Z, mvpVertex2.Z});
        RenderQueue.emplace_back(mvpVertex0, mvpVertex1);
        RenderQueue.emplace_back(mvpVertex1, mvpVertex2);
        RenderQueue.emplace_back(mvpVertex2, mvpVertex0);
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
    if (RenderQueue.empty())
        return;

    //Get edges
    HiddenLine hiddenLine;
    std::vector<Edge> visibleSegments = hiddenLine.EliminateHiddenLines(RenderQueue, {0.0f, 0.0f,0.0f});
    for (const auto &edge : visibleSegments)
    {
        Renderer2D::DrawLine(edge.start, edge.end, FVector3{1.0f, 1.0f, 1.0f});
    }
    ClearQueue();
}

void Renderer::ClearQueue()
{
    RenderQueue.clear();
}

bool Renderer::IsOnScreen(const FVector3 &point)
{
    return (point.X >= xNDCMin - EPSILON && point.X <= xNDCMax + EPSILON) &&
           (point.Y >= yNDCMin - EPSILON && point.Y <= yNDCMax + EPSILON);
}

bool Renderer::IsPointInsideEdge(const FVector2 &point, const FVector2 &edgeStart, const FVector2 &edgeEnd)
{
    return (edgeEnd.Y - edgeStart.Y) * point.X + (edgeStart.X - edgeEnd.X) * point.Y +
               (edgeEnd.X * edgeStart.Y - edgeStart.X * edgeEnd.Y) < -0.01;
}




FVector2 Renderer::ComputeIntersection(const FVector2 &edgeAStart, const FVector2 &edgeAEnd, const FVector2 &edgeBStart,
                                       const FVector2 &edgeBEnd)
{
    FVector2 dc = {edgeAStart.X - edgeAEnd.X, edgeAStart.Y - edgeAEnd.Y};
    FVector2 dp = {edgeBStart.X - edgeBEnd.X, edgeBStart.Y - edgeBEnd.Y};

    float n1 = edgeAStart.X * edgeAEnd.Y - edgeAStart.Y * edgeAEnd.X;
    float n2 = edgeBStart.X * edgeBEnd.Y - edgeBStart.Y * edgeBEnd.X;
    float n3 = 1.0f / (dc.X * dp.Y - dc.Y * dp.X);

    return {((n1 * dp.X - dc.X * n2) * n3), ((n1 * dp.Y - dc.Y * n2) * n3)};
}



std::vector<FVector2> Renderer::SutherlandHodgmanClip(const std::vector<FVector2> &subjectPolygon,
                                                      const std::vector<FVector2> &occluderPolygon)
{
    std::vector<FVector2> outputList(subjectPolygon);

    for (size_t i = 0; i < occluderPolygon.size(); i++)
    {
        FVector2 edgeStart = occluderPolygon[i];
        FVector2 edgeEnd = occluderPolygon[(i + 1) % occluderPolygon.size()];
        std::vector<FVector2> inputList(outputList);
        outputList.clear();

        for (size_t j = 0; j < inputList.size(); j++)
        {
            FVector2 currentVertex = inputList[j];
            FVector2 previousVertex = inputList[(j + inputList.size() - 1) % inputList.size()];
            if (currentVertex == edgeStart || currentVertex == edgeEnd)
            {
                //continue;
            }
            if (IsPointInsideEdge(currentVertex, edgeStart, edgeEnd))
            {
                if (!IsPointInsideEdge(previousVertex, edgeStart, edgeEnd))
                {
                    outputList.push_back(ComputeIntersection(edgeStart, edgeEnd, previousVertex, currentVertex));
                }
                outputList.push_back(currentVertex);
            }
            else if (IsPointInsideEdge(previousVertex, edgeStart, edgeEnd))
            {
                outputList.push_back(ComputeIntersection(edgeStart, edgeEnd, previousVertex, currentVertex));
            }
        }
    }
    return outputList;
}
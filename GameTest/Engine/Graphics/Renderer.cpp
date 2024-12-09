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
const float EPSILON = 0.05f;
std::vector<Edge> Renderer::RenderQueue;
std::vector<Face> Renderer::Faces;

void Renderer::QueueMesh(const Mesh &mesh, const Matrix4 &MVP, const Matrix4& normalMatrix )
{
    for (const auto &face : mesh.faces)
    {
        //Transformed vertices
        FVector3 mvpVertex0 = MVP.TransformWithPerspectiveDivide(face.v0);
        FVector3 mvpVertex1 = MVP.TransformWithPerspectiveDivide(face.v1);
        FVector3 mvpVertex2 = MVP.TransformWithPerspectiveDivide(face.v2);

        FVector3 viewNormal = (normalMatrix * face.normal).Normalize();

        // Within ndc space
        if (!IsOnScreen(mvpVertex0) && !IsOnScreen(mvpVertex1) && !IsOnScreen(mvpVertex2))
        {
            continue;
        }
        //Backface culling
        float determinant = ((mvpVertex1.X - mvpVertex0.X) * (mvpVertex2.Y - mvpVertex0.Y)) - (
                                (mvpVertex1.Y - mvpVertex0.Y) * (mvpVertex2.X - mvpVertex0.X));
        if (determinant < 0.0f)
        {
            //continue;
        }

        float dotProduct = viewNormal.Dot(VIEW_DIRECTION);
        
        if (dotProduct <= -EPSILON)
        {
            //continue;
        }
        if (viewNormal.Z < EPSILON)
        { 
            continue;
        }
        Faces.emplace_back(mvpVertex0, mvpVertex1, mvpVertex2,viewNormal);
        RenderQueue.push_back(Edge(mvpVertex0, mvpVertex1));
        RenderQueue.push_back(Edge(mvpVertex1, mvpVertex2));
        RenderQueue.push_back(Edge(mvpVertex2, mvpVertex0));
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
    return (point.X >= -1.0f && point.X <= 1.0f) &&
           (point.Y >= -1.0f && point.Y <= 1.0f);
}

bool Renderer::IsEdgeBehindFace(const Edge &edge, const Face &face)
{
    // Edge is hidden if both endpoints are behind
    return IsPointBehindFace(edge.start, face) && IsPointBehindFace(edge.end, face);
}

// Determines if a point in 3D space is behind a triangular face
// Returns true if point is behind face, false otherwise
// We can assume that the face is front facing
//All backfaces have already been culled
// We can also assume that the point will be used to comprise a front facing face
bool Renderer::IsPointBehindFace(const FVector3& point, const Face& face) {
    FVector3 vectorToPoint = face.v0 - point;  // Correct for +Z view
    return face.normal.Dot(vectorToPoint) < 0.0f;  // Correct sign for +Z view
}

bool Renderer::IntersectEdgeWithFace(const Edge &edge, const Face &face, FVector3 &intersectionOut)
{
    FVector3 edgeDirection = edge.end - edge.start;
    FVector3 faceNormal = face.normal;

    float denominator = faceNormal.Dot(edgeDirection);
    if (denominator == 0.0f)
    {
        return false;
    }
    float t = faceNormal.Dot(face.v0 - edge.start) / denominator;
    if (t < 0.0f || t > 1.0f)
    {
        return false;
    }
    intersectionOut = edge.start + edgeDirection * t;

    FVector3 edge1 = face.v1 - face.v0;
    FVector3 edge2 = face.v2 - face.v0;
    FVector3 point = intersectionOut - face.v0;


}


void Renderer::SubmitQueue()
{
    for (auto &edge : RenderQueue)
    {
        edge.isVisable = true;
        FVector3 color(1, 1, 1); // White = visible
        std::vector<FVector3> intersections;
        for (const auto &face : Faces)
        {
            bool startBehind = IsPointBehindFace(edge.start, face);
            bool endBehind = IsPointBehindFace(edge.end, face);

            if (startBehind && endBehind)
            {
                edge.isVisable = false;
                color = FVector3(1, 0, 0); // Red = hidden
                break;
            }
            else if (startBehind || endBehind)
            {
                color = FVector3(0, 1, 0); // Green = partially hidden
            }
        }

        Renderer2D::DrawLine({edge.start.X, edge.start.Y}, {edge.end.X, edge.end.Y}, color);
    }
    ClearQueue();
}

void Renderer::ClearQueue()
{
    RenderQueue.clear();
    Faces.clear();
}

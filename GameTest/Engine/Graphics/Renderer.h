#pragma once
#include "Camera.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector3.h"
struct Edge
{
    FVector3 start;
    FVector3 end;
    Edge(const FVector3 &start, const FVector3 &end) : start(start), end(end)
    {
    }
    bool isVisable = true;
};
class Renderer
{
public:
    static void QueueMesh(const Mesh &mesh, const Matrix4 &MVP,const Matrix4& normalMatrix);
    static void QueueModel(const Model &model, const Matrix4 &MVP,const Matrix4& normalMatrix);
    static void SubmitQueue();
    static void ClearQueue();

private:
    static bool IsOnScreen(const FVector3 &point);
    static bool IsEdgeBehindFace(const Edge &edge, const Face &face);
    static bool IsPointBehindFace(const FVector3 &point, const Face &face);
    static bool IntersectEdgeWithFace(const Edge &edge, const Face &face, FVector3& intersectionOut);
    static std::vector<Edge> RenderQueue;
    // All faces for this draw call are needed to check if edges are behind it
    static std::vector<Face> Faces;
    //this needs to be fixed because right now faces act as indices
    //Maybe make faces store the direct vertices
};

//Things we need to render something. MVP matrix, mesh, shader, camera
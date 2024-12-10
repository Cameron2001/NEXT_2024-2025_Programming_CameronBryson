#pragma once
#include "Camera.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector2.h"
enum ClipEdge
{
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
};
struct RenderFace
{
    FVector2 v0;
    FVector2 v1;
    FVector2 v2;
    float minDepth; // Used for depth sorting

    RenderFace(const FVector2 &v0, const FVector2 &v1, const FVector2 &v2, float depth)
        : v0(v0), v1(v1), v2(v2), minDepth(depth)
    {
    }
};

class Renderer
{
public:
    static void QueueMesh(const Mesh &mesh, const Matrix4 &MVP);
    static void QueueModel(const Model &model, const Matrix4 &MVP);
    static void SubmitQueue();
    static void ClearQueue();

private:
    static bool IsOnScreen(const FVector3 &point);

    static std::vector<RenderFace> QueuedFaces;

    // Sutherland-Hodgman Clipping Methods
    static std::vector<FVector2> ClipPolygon(const std::vector<FVector2> &subjectPolygon,
                                             const std::vector<std::vector<FVector2>> &occluders);
    static bool ComputeIntersection(const FVector2 &p1, const FVector2 &p2, ClipEdge edge, FVector2 &intersect);
    static std::vector<float> GetOcclusionPoints(const FVector2 &start, const FVector2 &end,
                                                 const std::vector<RenderFace> &occluders);
    static std::vector<std::pair<FVector2, FVector2>> GetVisibleSegments(const FVector2 &start, const FVector2 &end,
                                                                         const std::vector<RenderFace> &occluders);
};


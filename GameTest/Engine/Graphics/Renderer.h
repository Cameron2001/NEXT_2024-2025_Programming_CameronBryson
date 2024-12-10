#pragma once
#include "Camera.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector3.h"
enum ClipEdge
{
    LEFT,
    RIGHT,
    BOTTOM,
    TOP
};
struct RenderFace
{
    Face face;
    float averageDepth; // Used for depth sorting

    RenderFace(const Face &f, float depth) : face(f), averageDepth(depth)
    {
    }
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

    static std::vector<RenderFace> QueuedFaces;

    // Sutherland-Hodgman Clipping Methods
    static std::vector<FVector3> ClipPolygon(const std::vector<FVector3> &subjectPolygon,
                                             const std::vector<std::vector<FVector3>> &occluders);
    static bool ComputeIntersection(const FVector3 &p1, const FVector3 &p2, ClipEdge edge, FVector3 &intersect);
    static std::vector<float> GetOcclusionPoints(const FVector3 &start, const FVector3 &end,
                                                 const std::vector<RenderFace> &occluders);
    static std::vector<std::pair<FVector3, FVector3>> GetVisibleSegments(const FVector3 &start, const FVector3 &end,
                                                                         const std::vector<RenderFace> &occluders);
};


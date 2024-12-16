#pragma once
#include "Camera.h"
#include "Engine/Graphics/Edge.h"
#include "Engine/Graphics/Mesh.h"
#include "Engine/Graphics/Model.h"
#include "Engine/Math/Matrix4.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include <unordered_set>

class Renderer
{
public:
    static void QueueMesh(const Mesh &mesh, const Matrix4 &MVP);
    static void QueueModel(const Model &model, const Matrix4 &MVP);
    static void SubmitQueue();
    static void ClearQueue();

private:
    static std::vector<Face> m_triangles;
    static bool IsOnScreen(const FVector3 &point);
    static bool IsPointInsideEdge(const FVector2 &point, const FVector2& edgeStart, const FVector2& edgeEnd);


    // Sutherland-Hodgman Clipping Methods
    static std::vector<FVector2> SutherlandHodgmanClip(const std::vector<FVector2> &subjectPolygon, const std::vector<FVector2>&occluderPolygon);
    static FVector2 ComputeIntersection(const FVector2 &edgeAStart, const FVector2 &edgeAEnd, const FVector2 &edgeBStart,
                                    const FVector2 &edgeBEnd);
};


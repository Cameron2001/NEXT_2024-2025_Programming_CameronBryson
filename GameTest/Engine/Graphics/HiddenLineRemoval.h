#pragma once
#include "Edge.h"
#include "Triangle.h"
#include <Engine/Math/Quadtree.h>
#include <unordered_set>
#include <vector>
#include <ppl.h>
class HiddenLineRemoval
{
  public:
    HiddenLineRemoval(const std::vector<Triangle> &triangles);
    std::vector<Edge3D> removeHiddenLines();
    ;

  private:
    void initializeQuadtree();
    std::vector<Triangle> m_triangles;
    std::unique_ptr<Quadtree> m_quadtree;

    void sortTrianglesByDepth();

    bool getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3 &intersectionPoint) const;

    bool isPointInsideTriangle(const FVector3 &point, const Triangle &triangle) const;

    std::pair<Edge3D, Edge3D> splitEdge(const Edge3D &edge, const FVector3 &splitPoint) const;
    std::vector<Edge3D> clipEdgeAgainstTriangle(const Edge3D &edge, const Triangle &triangle) const;
    bool processEdge(const Edge3D &edge, const std::vector<Triangle> &potentialOccluders,
                     std::vector<Edge3D> &visibleEdges);
};

inline bool HiddenLineRemoval::isPointInsideTriangle(const FVector3 &point, const Triangle &triangle) const
{
    FVector2 p(point.X, point.Y);
    FVector2 v0(triangle.v0.X, triangle.v0.Y);
    FVector2 v1(triangle.v1.X, triangle.v1.Y);
    FVector2 v2(triangle.v2.X, triangle.v2.Y);

    // Compute vectors
    float denom = (v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y);
    if (fabs(denom) < 1e-6f)
        return false; // Degenerate triangle

    float a = ((v1.Y - v2.Y) * (p.X - v2.X) + (v2.X - v1.X) * (p.Y - v2.Y)) / denom;
    float b = ((v2.Y - v0.Y) * (p.X - v2.X) + (v0.X - v2.X) * (p.Y - v2.Y)) / denom;
    float c = 1.0f - a - b;

    return (a >= 0.0f) && (b >= 0.0f) && (c >= 0.0f);
}
inline std::pair<Edge3D, Edge3D> HiddenLineRemoval::splitEdge(const Edge3D &edge, const FVector3 &splitPoint) const
{
    Edge3D edgeA(edge.start, splitPoint);
    Edge3D edgeB(splitPoint, edge.end);
    return {edgeA, edgeB};
}

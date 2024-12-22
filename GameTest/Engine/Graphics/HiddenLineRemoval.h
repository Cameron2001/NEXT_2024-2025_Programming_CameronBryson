// HiddenLineRemoval.h
#pragma once

#include "Edge.h"
#include "Triangle.h"
#include <Engine/Math/Quadtree.h>
#include <unordered_set>
#include <vector>
#include <memory>
#include <concurrent_vector.h>
class FVector3;
class FVector2;
class BoundingBox2D;

struct Edge3DHash; // Forward declaration

class HiddenLineRemoval
{
  public:
    HiddenLineRemoval(const std::vector<Triangle> &triangles);
    std::vector<Edge3D> removeHiddenLines() const;

  private:
    // Initialization and sorting
    void initializeQuadtree();
    void sortTrianglesByDepth();

    // Intersection and clipping
    static bool getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3 &intersectionPoint);
    static std::vector<Edge3D> clipEdgeAgainstTriangle(const Edge3D &edge, const Triangle &triangle);

    // Point inside triangle test
    static bool isPointInsideTriangle(const FVector3 &point, const Triangle &triangle);

    // Processing functions
    static std::vector<Edge3D> processEdge(const Edge3D &edge, const std::vector<Triangle> &potentialOccluders);
    void processTriangle(const Triangle &triangle, std::unordered_set<Edge3D, Edge3DHash> &uniqueEdges,
                         std::vector<Edge3D> &visibleEdges) const;
    static std::vector<Edge3D> createTriangleEdges(const Triangle &triangle);
    static bool sharesVertex(const Triangle &occluder, const Edge3D &edge);
    static std::vector<Edge3D> clipSegmentsWithOccluder(const std::vector<Edge3D> &segments, const Triangle &occluder);
    static void appendVisibleSegments(std::vector<Edge3D> &visibleEdges, const std::vector<Edge3D> &segments);
    // Member variables
    std::vector<Triangle> m_triangles;
    std::unique_ptr<Quadtree> m_quadtree;
};

inline bool HiddenLineRemoval::isPointInsideTriangle(const FVector3 &point, const Triangle &triangle)
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
// Check if an occluder shares any vertex with the edge
inline bool HiddenLineRemoval::sharesVertex(const Triangle &occluder, const Edge3D &edge)
{
    return (occluder.v0 == edge.start || occluder.v0 == edge.end || occluder.v1 == edge.start ||
            occluder.v1 == edge.end || occluder.v2 == edge.start || occluder.v2 == edge.end);
}
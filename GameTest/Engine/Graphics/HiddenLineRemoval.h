
#pragma once
#include "Edge.h"
#include "Triangle.h"
#include <cmath>
#include <Engine/Math/Quadtree.h>
#include <memory>
#include <unordered_set>
#include <vector>
class FVector3;
class FVector2;
class BoundingBox2D;

struct Edge2DHash;
struct Edge3DHash;

class HiddenLineRemoval
{
  public:
    explicit HiddenLineRemoval(const std::vector<Triangle2D> &triangles);
    ~HiddenLineRemoval() = default;
    HiddenLineRemoval(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval &operator=(const HiddenLineRemoval &) = delete;

    HiddenLineRemoval(HiddenLineRemoval &&) = default;
    HiddenLineRemoval &operator=(HiddenLineRemoval &&) = default;
    std::vector<Edge2D> removeHiddenLines() const;

  private:
    // Initialization and sorting
    void initializeQuadtree();
    void sortTrianglesByDepth();

    // Intersection and clipping
    static bool getEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB, FVector2 &intersectionPoint);
    static std::vector<Edge2D> clipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle);

    // Point inside triangle test
    static bool isPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle);

    // Processing functions
    static std::vector<Edge2D> processEdge(const Edge2D &edge, const std::vector<Triangle2D> &potentialOccluders);
    void processTriangle(const Triangle2D &triangle, std::unordered_set<Edge2D, Edge2DHash> &uniqueEdges,
                         std::vector<Edge2D> &visibleEdges) const;
    static std::vector<Edge2D> createTriangleEdges(const Triangle2D &triangle);
    static bool sharesVertex(const Triangle2D &occluder, const Edge2D &edge);
    static std::vector<Edge2D> clipSegmentsWithOccluder(const std::vector<Edge2D> &segments,
                                                        const Triangle2D &occluder);
    static void appendVisibleSegments(std::vector<Edge2D> &visibleEdges, const std::vector<Edge2D> &segments);
    // Member variables
    std::vector<Triangle2D> m_triangles;
    std::unique_ptr<Quadtree> m_quadtree;
};

inline bool HiddenLineRemoval::isPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle)
{
    const FVector2 p(point.X, point.Y);
    const FVector2 v0(triangle.v0.X, triangle.v0.Y);
    const FVector2 v1(triangle.v1.X, triangle.v1.Y);
    const FVector2 v2(triangle.v2.X, triangle.v2.Y);

    // Compute vectors
    const float denom = (v1.Y - v2.Y) * (v0.X - v2.X) + (v2.X - v1.X) * (v0.Y - v2.Y);
    if (fabs(denom) < 1e-6f)
        return false; // Degenerate triangle

    const float a = ((v1.Y - v2.Y) * (p.X - v2.X) + (v2.X - v1.X) * (p.Y - v2.Y)) / denom;
    const float b = ((v2.Y - v0.Y) * (p.X - v2.X) + (v0.X - v2.X) * (p.Y - v2.Y)) / denom;
    const float c = 1.0f - a - b;

    return (a >= 0.0f) && (b >= 0.0f) && (c >= 0.0f);
}
// Check if an occluder shares any vertex with the edge
inline bool HiddenLineRemoval::sharesVertex(const Triangle2D &occluder, const Edge2D &edge)
{
    return (occluder.v0 == edge.start || occluder.v0 == edge.end || occluder.v1 == edge.start ||
            occluder.v1 == edge.end || occluder.v2 == edge.start || occluder.v2 == edge.end);
}

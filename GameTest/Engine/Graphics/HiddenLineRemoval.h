#pragma once

#include "Edge.h"
#include "Triangle.h"
#include <Engine/Math/Quadtree.h>
#include <Engine/Math/BoundingBox.h>
#include <memory>
#include <unordered_set>
#include <vector>

class FVector2;

struct Edge2DHash;

class HiddenLineRemoval
{
  public:
    HiddenLineRemoval();
    ~HiddenLineRemoval() = default;

    HiddenLineRemoval(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval &operator=(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval(HiddenLineRemoval &&) = default;
    HiddenLineRemoval &operator=(HiddenLineRemoval &&) = default;

    std::vector<Edge2D> removeHiddenLines(std::vector<Triangle2D> &triangles);

  private:
    void initializeQuadtree(const std::vector<Triangle2D> &triangles);
    void processTriangle(const Triangle2D &triangle);
    void processEdge(const Edge2D &edge, const std::vector<Triangle2D> &occluders);
    void clipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle);

    static bool getEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB, FVector2 &intersectionPoint);
    static bool isPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle);
    static void createTriangleEdges(const Triangle2D &triangle, Edge2D edges[3]);
    static bool sharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB);

    std::unique_ptr<Quadtree> m_quadtree;
    std::unordered_set<Edge2D, Edge2DHash> m_uniqueEdges;
    std::vector<Edge2D> m_visibleEdges;

    // Reusable buffers to avoid dynamic allocations
    std::vector<FVector2> m_intersectionPoints;
    std::vector<FVector2> m_sortedPoints;
    std::vector<Edge2D> m_segments;
    std::vector<Edge2D> m_clippedEdges;
    std::vector<Triangle2D> m_potentialOccluders;
};

inline void HiddenLineRemoval::createTriangleEdges(const Triangle2D &triangle, Edge2D edges[3])
{
    edges[0] = Edge2D(triangle.v0, triangle.v1);
    edges[1] = Edge2D(triangle.v1, triangle.v2);
    edges[2] = Edge2D(triangle.v2, triangle.v0);
}

inline bool HiddenLineRemoval::sharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB)
{
    return (triangleA.v0 == triangleB.v0 || triangleA.v0 == triangleB.v1 || triangleA.v0 == triangleB.v2 ||
            triangleA.v1 == triangleB.v0 || triangleA.v1 == triangleB.v1 || triangleA.v1 == triangleB.v2 ||
            triangleA.v2 == triangleB.v0 || triangleA.v2 == triangleB.v1 || triangleA.v2 == triangleB.v2);
}

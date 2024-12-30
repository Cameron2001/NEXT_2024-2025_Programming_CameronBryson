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
    void processTriangle(const Triangle2D &triangle, std::unordered_set<Edge2D, Edge2DHash> &uniqueEdges,
                         std::vector<Edge2D> &visibleEdges) const;
    static std::vector<Edge2D> createTriangleEdges(const Triangle2D &triangle);
    static std::vector<Edge2D> processEdge(const Edge2D &edge, const std::vector<Triangle2D> &potentialOccluders);
    static bool sharesVertex(const Triangle2D &occluder, const Edge2D &edge);
    static bool sharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB);
    static std::vector<Edge2D> clipSegmentsWithOccluder(const std::vector<Edge2D> &segments,
                                                        const Triangle2D &occluder);
    static void appendVisibleSegments(std::vector<Edge2D> &visibleEdges, const std::vector<Edge2D> &segments);
    // Member variables
    std::vector<Triangle2D> m_triangles;
    std::unique_ptr<Quadtree> m_quadtree;
};

inline bool HiddenLineRemoval::isPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle)
{
    float deltaX = point.X - triangle.v0.X;
    float deltaY = point.Y - triangle.v0.Y;

    bool isSideABPositive = (triangle.v1.X - triangle.v0.X) * deltaY - (triangle.v1.Y - triangle.v0.Y) * deltaX > 0;

    if (((triangle.v2.X - triangle.v0.X) * deltaY - (triangle.v2.Y - triangle.v0.Y) * deltaX > 0) == isSideABPositive)
        return false;
    if (((triangle.v2.X - triangle.v1.X) * (point.Y - triangle.v1.Y) -
             (triangle.v2.Y - triangle.v1.Y) * (point.X - triangle.v1.X) >
         0) != isSideABPositive)
        return false;
    return true;
}

// Check if an occluder shares any vertex with the edge
inline bool HiddenLineRemoval::sharesVertex(const Triangle2D &occluder, const Edge2D &edge)
{
    return (occluder.v0 == edge.start || occluder.v0 == edge.end || occluder.v1 == edge.start ||
            occluder.v1 == edge.end || occluder.v2 == edge.start || occluder.v2 == edge.end);
}

inline bool HiddenLineRemoval::sharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB)
{
    return (triangleA.v0 == triangleB.v0 || triangleA.v0 == triangleB.v1 || triangleA.v0 == triangleB.v2 ||
            triangleA.v1 == triangleB.v0 || triangleA.v1 == triangleB.v1 || triangleA.v1 == triangleB.v2 ||
            triangleA.v2 == triangleB.v0 || triangleA.v2 == triangleB.v1 || triangleA.v2 == triangleB.v2);
}

inline bool HiddenLineRemoval::getEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB,
                                                   FVector2 &intersectionPoint)
{
    const float startAx = edgeA.start.X;
    const float startAy = edgeA.start.Y;
    const float endAx = edgeA.end.X;
    const float endAy = edgeA.end.Y;
    const float startBx = edgeB.start.X;
    const float startBy = edgeB.start.Y;
    const float endBx = edgeB.end.X;
    const float endBy = edgeB.end.Y;

    const float denominator = (startAx - endAx) * (startBy - endBy) - (startAy - endAy) * (startBx - endBx);
    constexpr float EPSILON = 1e-6f;

    if (fabs(denominator) < EPSILON)
    {
        const bool isStartAStartB = edgeA.start == edgeB.start;
        const bool isStartAEndB = edgeA.start == edgeB.end;
        const bool isEndAStartB = edgeA.end == edgeB.start;
        const bool isEndAEndB = edgeA.end == edgeB.end;

        const int overlapCount = isStartAStartB + isStartAEndB + isEndAStartB + isEndAEndB;

        if (overlapCount > 1)
            return false;
        else if (overlapCount == 1)
        {
            if (isStartAStartB || isStartAEndB)
                intersectionPoint = edgeA.start;
            else
                intersectionPoint = edgeA.end;
            return true;
        }
        else
            return false;
    }

    const float numeratorT = (startAx - startBx) * (startBy - endBy) - (startAy - startBy) * (startBx - endBx);
    const float numeratorU = (startAx - startBx) * (startAy - endAy) - (startAy - startBy) * (startAx - endAx);
    const float t = numeratorT / denominator;
    const float u = numeratorU / denominator;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false;

    const float intersectionX = startAx + t * (endAx - startAx);
    const float intersectionY = startAy + t * (endAy - startAy);

    intersectionPoint = FVector2(intersectionX, intersectionY);
    return true;
}

#pragma once

#include "Edge.h"
#include "Triangle.h"
#include <Engine/Math/Quadtree.h>
#include <memory>
#include <ppl.h>
#include <vector>
#include "Engine/Core/Semaphore.h"
#include <mutex>
class FVector2;

struct BufferContext
{
    std::vector<Triangle2D> potentialOccluders;
    std::vector<Edge2D> segments;
    std::vector<Edge2D> newClippedEdges;
    // we dont need a visable segments buffer

    BufferContext()
    {
        potentialOccluders.reserve(100);
        segments.reserve(100);
        newClippedEdges.reserve(50);
    }

    void clear()
    {
        potentialOccluders.clear();
        segments.clear();
        newClippedEdges.clear();
    }
};

class HiddenLineRemoval
{
  public:
    HiddenLineRemoval();
    ~HiddenLineRemoval() = default;

    HiddenLineRemoval(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval &operator=(const HiddenLineRemoval &) = delete;
    HiddenLineRemoval(HiddenLineRemoval &&) = default;
    HiddenLineRemoval &operator=(HiddenLineRemoval &&) = default;

    std::vector<Edge2D> RemoveHiddenLines(std::vector<Triangle2D> &triangles);

  private:
    void InitializeQuadtree(const std::vector<Triangle2D> &triangles);
    void ProcessTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &potentialOccluders,
                         std::vector<Edge2D> &segments, std::vector<Edge2D> &newClippedEdges);

    void ProcessEdge(const Edge2D &edge, const std::vector<Triangle2D> &occluders, std::vector<Edge2D> &segments,
                     std::vector<Edge2D> &newClippedEdges);

    void ClipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle, std::vector<Edge2D> &clippedEdges);

    static bool GetEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB, FVector2 &intersectionPoint);
    static bool IsPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle);
    static void CreateTriangleEdges(const Triangle2D &triangle, Edge2D edges[3]);
    static bool SharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB);

    Semaphore m_semaphore;
    concurrency::combinable<std::vector<Edge2D>> m_visibleEdges;
    std::vector<std::unique_ptr<BufferContext>> m_threadBuffers;
    std::mutex m_bufferMutex;
    std::vector<Edge2D> m_result;
    std::unique_ptr<Quadtree> m_quadtree;
    size_t m_bufferPoolSize = 6;
};

inline void HiddenLineRemoval::CreateTriangleEdges(const Triangle2D &triangle, Edge2D edges[3])
{
    edges[0] = Edge2D(triangle.v0, triangle.v1);
    edges[1] = Edge2D(triangle.v1, triangle.v2);
    edges[2] = Edge2D(triangle.v2, triangle.v0);
}

inline bool HiddenLineRemoval::SharesVertex(const Triangle2D &triangleA, const Triangle2D &triangleB)
{
    return (triangleA.v0 == triangleB.v0 || triangleA.v0 == triangleB.v1 || triangleA.v0 == triangleB.v2 ||
            triangleA.v1 == triangleB.v0 || triangleA.v1 == triangleB.v1 || triangleA.v1 == triangleB.v2 ||
            triangleA.v2 == triangleB.v0 || triangleA.v2 == triangleB.v1 || triangleA.v2 == triangleB.v2);
}

inline bool HiddenLineRemoval::IsPointInsideTriangle(const FVector2 &point, const Triangle2D &triangle)
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

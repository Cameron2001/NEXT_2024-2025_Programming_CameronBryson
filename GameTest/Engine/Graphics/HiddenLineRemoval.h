#pragma once
#include "Edge.h"
#include "Face.h"
#include <Engine/Math/Quadtree.h>
#include <unordered_set>
#include <vector>
class HiddenLineRemoval
{
  public:
    HiddenLineRemoval(const std::vector<Face> &triangles);
    std::vector<Edge3D> removeHiddenLines();
    ;

  private:
    void initializeQuadtree();
    std::vector<Face> m_triangles;
    std::unique_ptr<Quadtree> m_quadtree;
    void sortTrianglesByDepth();

    bool getEdgeIntersection(const Edge3D &edgeA, const Edge3D &edgeB, FVector3 &intersectionPoint) const;

    bool isPointInsideTriangle(const FVector3 &point, const Face &triangle) const;
    float triangleArea(const Face &triangle) const;

    std::pair<Edge3D, Edge3D> splitEdge(const Edge3D &edge, const FVector3 &splitPoint) const;
    std::vector<Edge3D> clipEdgeAgainstTriangle(const Edge3D &edge, const Face &triangle) const;
    bool processTriangle(const Face &triangle, const std::vector<Face> &potentialOccluders,
                         std::unordered_set<Edge3D, Edge3DHash> &processedEdges, std::vector<Edge3D> &visibleEdges);
};

inline bool HiddenLineRemoval::isPointInsideTriangle(const FVector3 &point, const Face &triangle) const
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
inline bool HiddenLineRemoval::processTriangle(const Face &triangle, const std::vector<Face> &potentialOccluders,
                                               std::unordered_set<Edge3D, Edge3DHash> &processedEdges,
                                               std::vector<Edge3D> &visibleEdges)
{
    bool isOcclued = true;
    constexpr size_t NUM_EDGES = 3;
    const Edge3D edges[NUM_EDGES] = {
        {triangle.v0, triangle.v1}, {triangle.v1, triangle.v2}, {triangle.v2, triangle.v0}};

    for (size_t i = 0; i < NUM_EDGES; ++i)
    {
        const auto &edge = edges[i];
        if (!processedEdges.insert(edge).second)
            continue; // Edge already processed
        std::vector<Edge3D> segments = {edge};
        for (const auto &occluder : potentialOccluders)
        {
            if (segments.empty())
                break;
            std::vector<Edge3D> tempSegments;
            tempSegments.reserve(segments.size());

            for (const auto &segment : segments)
            {
                auto clipped = clipEdgeAgainstTriangle(segment, occluder);
                tempSegments.insert(tempSegments.end(), clipped.begin(), clipped.end());
            }

            segments = std::move(tempSegments);
        }

        // Bulk insert visible segments to reduce overhead
        if (!segments.empty())
        {
            visibleEdges.insert(visibleEdges.end(), std::make_move_iterator(segments.begin()),
                                std::make_move_iterator(segments.end()));
            isOcclued = false;
        }
    }
    return isOcclued;
}
inline float HiddenLineRemoval::triangleArea(const Face &triangle) const
{
    // Calculate the area using the cross product method
    FVector3 vec1 = triangle.v1 - triangle.v0;
    FVector3 vec2 = triangle.v2 - triangle.v0;
    float area = 0.5f * vec1.Cross(vec2).Length();
    return area;
}

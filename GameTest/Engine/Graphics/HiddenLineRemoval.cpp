#include "stdafx.h"
#include "Engine/Graphics/Edge.h"
#include "Engine/Math/BoundingBox.h"
#include "Engine/Math/Vector2.h"
#include "HiddenLineRemoval.h"

#include "Triangle.h"
#include <algorithm>
#include <cstdlib>
#include <Engine/Math/Quadtree.h>
#include <limits>
#include <memory>
#include <mutex>
#include <ppl.h>
#include <type_traits>
#include <utility>
#include <vector>
#include "Engine/Core/SemaphoreGuard.h"

HiddenLineRemoval::HiddenLineRemoval()
{
    for (size_t i = 0; i < m_bufferPoolSize; ++i)
    {
        m_threadBuffers.emplace_back(std::make_unique<BufferContext>());
        m_semaphore.Release();
    }
    m_result.reserve(10000);
    m_visibleEdges.local().reserve(10000);
}

std::vector<Edge2D> HiddenLineRemoval::RemoveHiddenLines(std::vector<Triangle2D> &triangles)
{
    InitializeQuadtree(triangles);
    for (const auto &triangle : triangles)
    {
        m_quadtree->Insert(triangle);
    }

    size_t totalTriangles = triangles.size();
    size_t batchSize = totalTriangles / m_bufferPoolSize;
    size_t numBatches = (totalTriangles + batchSize - 1) / batchSize;

    concurrency::parallel_for(size_t(0), numBatches, [&](size_t batchIndex) {
        size_t startIdx = batchIndex * batchSize;
        size_t endIdx = std::min(startIdx + batchSize, totalTriangles);
        size_t currentBatchSize = endIdx - startIdx;

        // Acquire a buffer for the entire batch
        SemaphoreGuard guard(m_semaphore);

        std::unique_ptr<BufferContext> buffer;
        {
            std::lock_guard<std::mutex> lock(m_bufferMutex);
            if (m_threadBuffers.empty())
            {
                // This should not happen due to semaphore control
                return;
            }
            buffer = std::move(m_threadBuffers.back());
            m_threadBuffers.pop_back();
        }

        for (size_t i = startIdx; i < endIdx; ++i)
        {
            const Triangle2D &triangle = triangles[i];
            ProcessTriangle(triangle, buffer->potentialOccluders, buffer->segments);
        }

        // Return the buffer to the pool
        {
            std::lock_guard<std::mutex> lock(m_bufferMutex);
            m_threadBuffers.emplace_back(std::move(buffer));
        }
    });

    m_result.clear();
    m_visibleEdges.combine_each([&](const std::vector<Edge2D> &localEdges) {
        for (const auto &edge : localEdges)
        {
            m_result.emplace_back(edge);
        }
    });
    m_visibleEdges.clear();
    return m_result;
}

void HiddenLineRemoval::InitializeQuadtree(const std::vector<Triangle2D> &triangles)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto &triangle : triangles)
    {
        minX = std::min(minX, std::min({triangle.v0.X, triangle.v1.X, triangle.v2.X}));
        minY = std::min(minY, std::min({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y}));
        maxX = std::max(maxX, std::max({triangle.v0.X, triangle.v1.X, triangle.v2.X}));
        maxY = std::max(maxY, std::max({triangle.v0.Y, triangle.v1.Y, triangle.v2.Y}));
    }

    BoundingBox2D rootBounds(minX, minY, maxX, maxY);
    m_quadtree = std::make_unique<Quadtree>(rootBounds, 4, 4);
}

void HiddenLineRemoval::ProcessTriangle(const Triangle2D &triangle, std::vector<Triangle2D> &potentialOccluders,
                                        std::vector<Edge2D> &segments)
{
    potentialOccluders.clear();
    m_quadtree->QueryTriangle(triangle, potentialOccluders);

    potentialOccluders.erase(
        std::remove_if(potentialOccluders.begin(), potentialOccluders.end(),
                       [&](const Triangle2D &occluder) { return SharesVertex(occluder, triangle); }),
        potentialOccluders.end());

    Edge2D edges[3];
    CreateTriangleEdges(triangle, edges);

    for (int i = 0; i < 3; ++i)
    {
        const Edge2D &edge = edges[i];
        ProcessEdge(edge, potentialOccluders, segments);
    }
}

void HiddenLineRemoval::ProcessEdge(const Edge2D &edge, const std::vector<Triangle2D> &occluders,
                                    std::vector<Edge2D> &segments)
{
    segments.clear();
    segments.emplace_back(edge);
    size_t currentIndex = 0;

    for (const auto &occluder : occluders)
    {
        size_t endIndex = segments.size();

        // Process segments from currentIndex to endIndex
        for (size_t i = currentIndex; i < endIndex; ++i)
        {
            const Edge2D &segment = segments[i];
            ClipEdgeAgainstTriangle(segment, occluder, segments);
        }

        // Update currentIndex to endIndex for the next occluder
        currentIndex = endIndex;

        // If no new segments were added, exit early
        if (currentIndex >= segments.size())
        {
            break;
        }
    }

    for (size_t i = currentIndex; i < segments.size(); ++i)
    {
        m_visibleEdges.local().emplace_back(segments[i]);
    }
}

void HiddenLineRemoval::ClipEdgeAgainstTriangle(const Edge2D &edge, const Triangle2D &triangle,
                                                std::vector<Edge2D> &segments)
{
    FVector2 intersectionPoints[2];
    size_t intersectionCount = 0;
    FVector2 sortedPoints[4];
    size_t sortedCount = 0;

    Edge2D triEdges[3];
    CreateTriangleEdges(triangle, triEdges);

    for (int i = 0; i < 3 && intersectionCount < 2; ++i)
    {
        FVector2 intersectionPoint;
        if (GetEdgeIntersection(edge, triEdges[i], intersectionPoint))
        {
            intersectionPoints[intersectionCount++] = intersectionPoint;
        }
    }

    if (intersectionCount == 0)
    {
        if (IsPointInsideTriangle(edge.start, triangle) && IsPointInsideTriangle(edge.end, triangle))
        {
            return;
        }
        else
        {
            // m_visibleEdges.local().emplace_back(edge);
            segments.emplace_back(edge);
            return;
        }
    }

    sortedPoints[sortedCount++] = edge.start;
    for (size_t i = 0; i < intersectionCount; ++i)
    {
        sortedPoints[sortedCount++] = intersectionPoints[i];
    }
    sortedPoints[sortedCount++] = edge.end;

    std::sort(sortedPoints, sortedPoints + sortedCount, [&](const FVector2 &a, const FVector2 &b) {
        return (a - edge.start).LengthSquared() < (b - edge.start).LengthSquared();
    });

    for (size_t i = 0; i < sortedCount - 1; ++i)
    {
        FVector2 midPoint = (sortedPoints[i] + sortedPoints[i + 1]) * 0.5f;
        if (!IsPointInsideTriangle(midPoint, triangle))
        {
            FVector2 delta = sortedPoints[i + 1] - sortedPoints[i];
            if (delta.LengthSquared() >= 1e-5f)
            {
                // m_visibleEdges.local().emplace_back(sortedPoints[i], sortedPoints[i + 1]);
                segments.emplace_back(sortedPoints[i], sortedPoints[i + 1]);
            }
        }
    }
}

bool HiddenLineRemoval::GetEdgeIntersection(const Edge2D &edgeA, const Edge2D &edgeB, FVector2 &intersectionPoint)
{
    const float x1 = edgeA.start.X;
    const float y1 = edgeA.start.Y;
    const float x2 = edgeA.end.X;
    const float y2 = edgeA.end.Y;
    const float x3 = edgeB.start.X;
    const float y3 = edgeB.start.Y;
    const float x4 = edgeB.end.X;
    const float y4 = edgeB.end.Y;

    const float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    constexpr float EPSILON = 1e-6f;

    if (std::abs(denominator) < EPSILON)
    {
        return false;
    }

    const float numeratorA = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
    const float numeratorB = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
    const float t = numeratorA / denominator;
    const float u = numeratorB / denominator;

    if (t < -EPSILON || t > 1.0f + EPSILON || u < -EPSILON || u > 1.0f + EPSILON)
        return false;

    intersectionPoint.X = x1 + t * (x2 - x1);
    intersectionPoint.Y = y1 + t * (y2 - y1);
    return true;
}

#pragma once
#include "Engine/Math/Vector3.h"
#include <functional>
struct Triangle
{
    Triangle(const FVector3 &v0, const FVector3 &v1, const FVector3 &v2) : v0(v0), v1(v1), v2(v2)
    {
        isFrontFacing = false;
    }
    Triangle(const Triangle &other, bool isFrontFacing)
        : v0(other.v0), v1(other.v1), v2(other.v2), isFrontFacing(isFrontFacing)
    {
    }
    Triangle(const Triangle &other) noexcept = default;
    Triangle &operator=(const Triangle &other) noexcept = default;

    Triangle(Triangle &&other) noexcept = default;
    Triangle &operator=(Triangle &&other) noexcept = default;
    bool operator==(const Triangle &other) const
    {
        return v0 == other.v0 && v1 == other.v1 && v2 == other.v2;
    }
    FVector3 v0;
    FVector3 v1;
    FVector3 v2;
    mutable bool isFrontFacing;
};

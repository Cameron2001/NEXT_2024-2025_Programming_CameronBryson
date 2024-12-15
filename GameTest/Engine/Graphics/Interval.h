#pragma once
struct Interval
{
    float start;
    float end;
    bool operator==(const Interval &other) const
    {
        return start == other.start && end == other.end;
    }
    Interval &operator=(const Interval &other)
    {
        start = other.start;
        end = other.end;
        return *this;
    }
};

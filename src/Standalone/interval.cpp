#include "interval.h"
#include "common_defines.h"

const FInterval FInterval::Empty = {+Infinity, -Infinity};
const FInterval FInterval::Universe = {-Infinity, +Infinity};

FInterval::FInterval() : Min(-Infinity), Max(+Infinity) {};

FInterval::FInterval(double MinIn, double MaxIn) : Min(MinIn), Max(MaxIn) {};

double FInterval::Size()
{
    return Max - Min;
};

bool FInterval::Contains(double X)
{
    return (Min <= X) && (X <= Max);
};

bool FInterval::Surrounds(double X)
{
    return (Min < X) && (X < Max);
}

double FInterval::Clamp(double X) const
{
    if (X < Min) return Min;
    if (X > Max) return Max;
    return X;
}
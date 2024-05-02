#include "interval.h"
#include "common_defines.h"

const FInterval FInterval::Empty = {+Infinity, -Infinity};
const FInterval FInterval::Universe = {-Infinity, +Infinity};

FInterval::FInterval() : Min(-Infinity), Max(+Infinity) {};

FInterval::FInterval(float MinIn, float MaxIn) : Min(MinIn), Max(MaxIn) {};

float FInterval::Size()
{
    return Max - Min;
};

bool FInterval::Contains(float X)
{
    return (Min <= X) && (X <= Max);
};

bool FInterval::Surrounds(float X)
{
    return (Min < X) && (X < Max);
}
#include "interval.h"
#include "common_defines.h"

const FInterval FInterval::Empty = {+Infinity, -Infinity};
const FInterval FInterval::Universe = {-Infinity, +Infinity};

FInterval::FInterval() : Min(-Infinity), Max(+Infinity) {};

FInterval::FInterval(double MinIn, double MaxIn) : Min(MinIn), Max(MaxIn) {};

FInterval::FInterval(const FInterval& A, const FInterval& B)
{
    Min = A.Min <= B.Min ? A.Min : B.Min;
    Max = A.Max >= B.Max ? A.Max : B.Max;
}

double FInterval::Size() const
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

FInterval FInterval::Expand(double Delta)
{
    auto Padding = Delta * 0.5;
    return {Min - Padding, Max + Padding};
}
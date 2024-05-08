#include "common_defines.h"
#include "rng.h"

#include "pdf.h"

double FSpherePDF::Value(const FVector3 Direction) const
{
    return 0.25 * M_PI_INV;
}

FVector3 FSpherePDF::Generate() const
{
    return RandomUnitVector();
}

FCosinePDF::FCosinePDF(const FVector3 &W)
{
    UVW.BuildFromW(W);
}

double FCosinePDF::Value(const FVector3 Direction) const
{
    auto CosTheta = Dot(Direction.GetNormalized(), UVW[2]);
    return fmax(0, CosTheta * M_PI_INV);
}

FVector3 FCosinePDF::Generate() const
{
    return UVW.Local(RandomCosineDirection());
}

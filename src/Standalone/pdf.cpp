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

FMixturePDF::FMixturePDF(std::shared_ptr<FPDF> P0, std::shared_ptr<FPDF> P1)
{
    P[0] = P0;
    P[1] = P1;
}

double FMixturePDF::Value(const FVector3 Direction) const
{
    return 0.5 * P[0]->Value(Direction) + 0.5 * P[1]->Value(Direction);
}

FVector3 FMixturePDF::Generate() const
{
    if (RandomDouble() < .5)
    {
        return P[0]->Generate();
    }
    else
    {
        return P[1]->Generate();
    }
}

#include "rng.h"

#include "vector3.h"

RNG1D::RNG1D() : RNG1D(0.f, 1.f) {};

RNG1D::RNG1D(float Min, float Max)
{
    Distribution = std::uniform_real_distribution<float>(Min, Max);
    Generator = std::mt19937();
};

float RNG1D::operator()()
{
    return Distribution(Generator);
};

RNG2D::RNG2D() : RNG2D(0.f, 1.f, 0.f, 1.f) {};

RNG2D::RNG2D(float MinX, float MaxX, float MinY, float MaxY)
{
    DistributionX = std::uniform_real_distribution<float>(MinX, MaxX);
    DistributionY = std::uniform_real_distribution<float>(MinY, MaxY);
    Generator = std::mt19937();
};

std::tuple<float, float> RNG2D::operator()()
{
    return std::make_tuple(DistributionX(Generator), DistributionY(Generator));
};

RNG3D::RNG3D() : RNG3D(0.f, 1.f, 0.f, 1.f, 0.f, 1.f) {};

RNG3D::RNG3D(float MinX, float MaxX, float MinY, float MaxY, float MinZ, float MaxZ)
{
    DistributionX = std::uniform_real_distribution<float>(MinX, MaxX);
    DistributionY = std::uniform_real_distribution<float>(MinY, MaxY);
    DistributionZ = std::uniform_real_distribution<float>(MinZ, MaxZ);
    Generator = std::mt19937();
};

std::tuple<float, float, float> RNG3D::operator()()
{
    return std::make_tuple(DistributionX(Generator), DistributionY(Generator), DistributionZ(Generator));
};

FVector3 RandomVector3()
{
    static RNG3D RNG3;
    auto [X, Y, Z] = RNG3();
    return FVector3{X, Y, Z};
};

FVector3 RandomVector3(float Min, float Max)
{
    auto RandomVector = RandomVector3();
    RandomVector = FVector3(Min, Min, Min) + ((FVector3(Max, Max, Max) - FVector3(Min, Min, Min))) * RandomVector;
    return RandomVector;
};

FVector3 RandomVectorInUnitSphere()
{
    while (true)
    {
        auto Candidate = RandomVector3(-1, 1);
        if (Candidate.Length2() < 1)
        {
            return Candidate;
        }
    }
};

FVector3 RandomUnitVector()
{
    return RandomVectorInUnitSphere().GetNormalized();
};

FVector3 RandomUnitVectorOnHemisphere(const FVector3& Normal)
{
    auto OnUnitSphere = RandomUnitVector();

    if (Dot(OnUnitSphere, Normal) > 0.f)
    {
        return OnUnitSphere;
    }
    else
    {
        return -OnUnitSphere;
    }
};

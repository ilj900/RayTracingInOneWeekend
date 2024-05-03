#include "rng.h"

#include "vector3.h"

RNG1D::RNG1D() : RNG1D(0.f, 1.f) {};

RNG1D::RNG1D(double Min, double Max)
{
    Distribution = std::uniform_real_distribution<double>(Min, Max);
    Generator = std::mt19937();
};

double RNG1D::operator()()
{
    return Distribution(Generator);
};

RNG2D::RNG2D() : RNG2D(0.f, 1.f, 0.f, 1.f) {};

RNG2D::RNG2D(double MinX, double MaxX, double MinY, double MaxY)
{
    DistributionX = std::uniform_real_distribution<double>(MinX, MaxX);
    DistributionY = std::uniform_real_distribution<double>(MinY, MaxY);
    Generator = std::mt19937();
};

std::tuple<double, double> RNG2D::operator()()
{
    return std::make_tuple(DistributionX(Generator), DistributionY(Generator));
};

RNG3D::RNG3D() : RNG3D(0.f, 1.f, 0.f, 1.f, 0.f, 1.f) {};

RNG3D::RNG3D(double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ)
{
    DistributionX = std::uniform_real_distribution<double>(MinX, MaxX);
    DistributionY = std::uniform_real_distribution<double>(MinY, MaxY);
    DistributionZ = std::uniform_real_distribution<double>(MinZ, MaxZ);
    Generator = std::mt19937();
};

std::tuple<double, double, double> RNG3D::operator()()
{
    return std::make_tuple(DistributionX(Generator), DistributionY(Generator), DistributionZ(Generator));
};

double RandomDouble()
{
    static RNG1D RNG1;
    return RNG1();
}

double RandomDouble(double Min, double Max)
{
    static RNG1D RNG1;
    double Value = RNG1();
    return Min + (Max - Min) * Value;
}

FVector3 RandomVector3()
{
    static RNG3D RNG3;
    auto [X, Y, Z] = RNG3();
    return FVector3{X, Y, Z};
};

FVector3 RandomVector3(double Min, double Max)
{
    auto RandomVector = RandomVector3();
    RandomVector = FVector3(Min, Min, Min) + ((FVector3(Max, Max, Max) - FVector3(Min, Min, Min))) * RandomVector;
    return RandomVector;
};

FVector3 RandomVectorInUnitSphere()
{
    while (true)
    {
        auto Candidate = RandomVector3(-1., 1.);
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

FVector3 RandomInUnitDisc()
{
    while (true)
    {
        auto Candidate = FVector3(RandomDouble(-1., 1.), RandomDouble(-1., 1.), 0);
        if (Candidate.Length2() < 1.f)
        {
            return Candidate;
        }
    }
}

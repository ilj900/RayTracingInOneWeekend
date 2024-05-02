#ifndef RNG_H
#define RNG_H

#include <tuple>
#include <random>

class FVector3;

class RNG1D
{
public:
    RNG1D();
    RNG1D(float Min, float Max);

    float operator()();

private:
    std::uniform_real_distribution<float>  Distribution;
    std::mt19937 Generator;
};

class RNG2D
{
public:
    RNG2D();
    RNG2D(float MinX, float MaxX, float MinY, float MaxY);

    std::tuple<float, float> operator()();

private:
    std::uniform_real_distribution<float>  DistributionX;
    std::uniform_real_distribution<float>  DistributionY;
    std::mt19937 Generator;
};

class RNG3D
{
public:
    RNG3D();
    RNG3D(float MinX, float MaxX, float MinY, float MaxY, float MinZ, float MaxZ);

    std::tuple<float, float, float> operator()();

private:
    std::uniform_real_distribution<float>  DistributionX;
    std::uniform_real_distribution<float>  DistributionY;
    std::uniform_real_distribution<float>  DistributionZ;
    std::mt19937 Generator;
};

FVector3 RandomVector3();
FVector3 RandomVector3(float Min, float Max);
FVector3 RandomVectorInUnitSphere();
FVector3 RandomUnitVector();
FVector3 RandomUnitVectorOnHemisphere(const FVector3& Normal);

#endif // RNG_H
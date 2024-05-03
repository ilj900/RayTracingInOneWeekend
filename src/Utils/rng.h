#ifndef RNG_H
#define RNG_H

#include <tuple>
#include <random>

class FVector3;

class RNG1D
{
public:
    RNG1D();
    RNG1D(double Min, double Max);

    double operator()();

private:
    std::uniform_real_distribution<double>  Distribution;
    std::mt19937 Generator;
};

class RNG2D
{
public:
    RNG2D();
    RNG2D(double MinX, double MaxX, double MinY, double MaxY);

    std::tuple<double, double> operator()();

private:
    std::uniform_real_distribution<double>  DistributionX;
    std::uniform_real_distribution<double>  DistributionY;
    std::mt19937 Generator;
};

class RNG3D
{
public:
    RNG3D();
    RNG3D(double MinX, double MaxX, double MinY, double MaxY, double MinZ, double MaxZ);

    std::tuple<double, double, double> operator()();

private:
    std::uniform_real_distribution<double>  DistributionX;
    std::uniform_real_distribution<double>  DistributionY;
    std::uniform_real_distribution<double>  DistributionZ;
    std::mt19937 Generator;
};

double RandomDouble();
double RandomDouble(double Min, double Max);

FVector3 RandomVector3();
FVector3 RandomVector3(double Min, double Max);
FVector3 RandomVectorInUnitSphere();
FVector3 RandomUnitVector();
FVector3 RandomUnitVectorOnHemisphere(const FVector3& Normal);
FVector3 RandomInUnitDisc();

#endif // RNG_H
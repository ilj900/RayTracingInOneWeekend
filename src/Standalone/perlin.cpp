#include "rng.h"

#include "perlin.h"

FPerlin::FPerlin()
{
    RandFloat = new double[PointCount];

    for (int i = 0; i < PointCount; ++i)
    {
        RandFloat[i] = RandomDouble();
    }

    PermX = PerlinGeneratePerm();
    PermY = PerlinGeneratePerm();
    PermZ = PerlinGeneratePerm();
}

FPerlin::~FPerlin()
{
    delete[] RandFloat;
    delete[] PermX;
    delete[] PermY;
    delete[] PermZ;
}

double FPerlin::Noise(const FVector3& Point) const
{
    auto i = int(4 * Point.X) & 255;
    auto j = int(4 * Point.Y) & 255;
    auto k = int(4 * Point.Z) & 255;

    return RandFloat[PermX[i] ^ PermY[j] ^ PermZ[k]];
}

int* FPerlin::PerlinGeneratePerm()
{
    auto P = new int[PointCount];

    for (int i = 0; i < PointCount; ++i)
    {
        P[i] = i;
    }

    Permute(P, PointCount);

    return P;
}

void FPerlin::Permute(int* P, int N)
{
    for (int i = N - 1; i > 0; i--)
    {
        int Target = RandomInt(0, i);
        int Tmp = P[i];
        P[i] = P[Target];
        P[Target] = Tmp;
    }
}

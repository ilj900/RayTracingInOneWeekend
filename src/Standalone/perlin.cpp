#include "rng.h"

#include "perlin.h"

FPerlin::FPerlin()
{
    RandVector = new FVector3[PointCount];

    for (int i = 0; i < PointCount; ++i)
    {
        RandVector[i] = RandomVector3(-1., 1.).GetNormalized();
    }

    PermX = PerlinGeneratePerm();
    PermY = PerlinGeneratePerm();
    PermZ = PerlinGeneratePerm();
}

FPerlin::~FPerlin()
{
    delete[] RandVector;
    delete[] PermX;
    delete[] PermY;
    delete[] PermZ;
}

double FPerlin::Noise(const FVector3& Point) const
{
    auto U = Point.X - std::floor(Point.X);
    auto V = Point.Y - std::floor(Point.Y);
    auto W = Point.Z - std::floor(Point.Z);

    auto i = int(std::floor(Point.X));
    auto j = int(std::floor(Point.Y));
    auto k = int(std::floor(Point.Z));

    FVector3 C[2][2][2];

    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                C[di][dj][dk] = RandVector[PermX[(i + di) & 255] ^ PermY[(j + dj) & 255] ^ PermZ[(k + dk) & 255]];
            }
        }
    }

    return TrilinearInterp(C, U, V, W);
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

double FPerlin::TrilinearInterp(const FVector3 C[2][2][2], double U, double V, double W)
{
    auto UU = U * U * (3 - 2 * U);
    auto VV = V * V * (3 - 2 * V);
    auto WW = W * W * (3 - 2 * W);
    double Accum = 0.;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                FVector3 WeightV(U - i, V - j, W - k);
                Accum += (i * UU + (1 - i) * (1 - UU))
                       * (j * VV + (1 - j) * (1 - VV))
                       * (k * WW + (1 - k) * (1 - WW))
                       * Dot(C[i][j][k], WeightV);
            }
        }
    }

    return Accum;
}

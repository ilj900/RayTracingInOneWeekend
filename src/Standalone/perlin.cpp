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
    auto U = Point.X - std::floor(Point.X);
    auto V = Point.Y - std::floor(Point.Y);
    auto W = Point.Z - std::floor(Point.Z);
    U = U * U * (3 - 2 * U);
    V = V * V * (3 - 2 * V);
    W = W * W * (3 - 2 * W);

    auto i = int(std::floor(Point.X));
    auto j = int(std::floor(Point.Y));
    auto k = int(std::floor(Point.Z));

    double C[2][2][2];

    for (int di = 0; di < 2; di++)
    {
        for (int dj = 0; dj < 2; dj++)
        {
            for (int dk = 0; dk < 2; dk++)
            {
                C[di][dj][dk] = RandFloat[PermX[(i + di) & 255] ^ PermY[(j + dj) & 255] ^ PermZ[(k + dk) & 255]];
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

double FPerlin::TrilinearInterp(double C[2][2][2], double U, double V, double W)
{
    double Accum = 0.;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                Accum += (i * U + (1 - i) * (1 - U))
                       * (j * V + (1 - j) * (1 - V))
                       * (k * W + (1 - k) * (1 - W))
                       * C[i][j][k];
            }
        }
    }

    return Accum;
}

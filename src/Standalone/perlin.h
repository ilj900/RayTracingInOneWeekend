#pragma once

#include "vector3.h"

class FPerlin
{
public:
    FPerlin();
    ~FPerlin();
    double Noise(const FVector3& Point) const;
    double Turb(const FPoint3& Point, int Depth) const;

private:
    static const int PointCount = 256;
    FVector3* RandVector;
    int* PermX;
    int* PermY;
    int* PermZ;

    static int* PerlinGeneratePerm();
    static void Permute(int* P, int N);
    static double TrilinearInterp(const FVector3 C[2][2][2], double U, double V, double W);
};

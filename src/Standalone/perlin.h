#pragma once

#include "vector3.h"

class FPerlin
{
public:
    FPerlin();
    ~FPerlin();
    double Noise(const FVector3& Point) const;

private:
    static const int PointCount = 256;
    double* RandFloat;
    int* PermX;
    int* PermY;
    int* PermZ;

    static int* PerlinGeneratePerm();
    static void Permute(int* P, int N);
};

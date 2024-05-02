#include "rng.h"

RNG1D::RNG1D() : RNG1D(0.f, 1.f) {};

RNG1D::RNG1D(float Min, float Max)
{
    Distribution = std::uniform_real_distribution<float>(Min, Max);
    Generator = std::mt19937();
}

float RNG1D::operator()()
{
    return Distribution(Generator);
}
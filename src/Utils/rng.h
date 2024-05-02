#ifndef RNG_H
#define RNG_H

#include <tuple>
#include <random>

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

#endif // RNG_H
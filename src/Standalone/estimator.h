#pragma once

#include "color3.h"

#include <vector>

class FEstimator
{
public:
    FEstimator(uint32_t WidthIn, uint32_t HeightIn, uint32_t BucketsCountIn, double ThresholdIn);
    void Store(int X, int Y, const FColor3 & Value);
    void SaveAsEXR(const std::string& Name);
    void SaveAsBMP(const std::string& Name);

private:
    double LinearToGamma(double Value) const;

    uint32_t Width;
    uint32_t Height;
    uint32_t BucketsCount = 9;
    double Threshold = 0.25;

    std::vector<uint32_t> Counter;
    std::vector<std::vector<double>> ImagesData;
    std::vector<double> MaxValue;
};


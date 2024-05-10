#include "stb_image_write.h"
#include "tinyexr.h"

#include "interval.h"

#include "estimator.h"

#include <algorithm>
#include <stdexcept>

FEstimator::FEstimator(uint32_t WidthIn, uint32_t HeightIn, uint32_t BucketsCountIn, double ThresholdIn) : Width(WidthIn), Height(HeightIn), BucketsCount(BucketsCountIn), Threshold(ThresholdIn)
{
    if (BucketsCount % 2 == 0)
    {
        throw std::runtime_error("Number of buckets should be odd number");
    }

    Counter.resize(Width * Height);
    UnestimatedImageData.resize(BucketsCount);
    EstimatedImageData.resize(Width * Height * 3);
    MaxValue.resize(Width * Height * 3, 0.);

    for (uint32_t i = 0; i < BucketsCount; ++i)
    {
        UnestimatedImageData[i].resize(Width * Height * 4); /// Four components per pixel
    }
}

void FEstimator::Store(int X, int Y, const FColor3 & Value)
{
    auto PixelIndex = Y * Width + X;

    if (MaxValue[PixelIndex * 3] < Value.X)
    {
        MaxValue[PixelIndex * 3] = Value.X;
    }

    if (MaxValue[PixelIndex * 3 + 1] < Value.Y)
    {
        MaxValue[PixelIndex * 3 + 1] = Value.Y;
    }

    if (MaxValue[PixelIndex * 3 + 2] < Value.Z)
    {
        MaxValue[PixelIndex * 3 + 2] = Value.Z;
    }

    UnestimatedImageData[Counter[PixelIndex]][PixelIndex * 4] += Value.X;
    UnestimatedImageData[Counter[PixelIndex]][PixelIndex * 4 + 1] += Value.Y;
    UnestimatedImageData[Counter[PixelIndex]][PixelIndex * 4 + 2] += Value.Z;
    UnestimatedImageData[Counter[PixelIndex]][PixelIndex * 4 + 3] += 1.;

    Counter[PixelIndex] = (Counter[PixelIndex] + 1) % BucketsCount;
}

void FEstimator::DoEstimate()
{
    for (uint32_t y = 0; y < Height; ++y)
    {
        for (uint32_t x = 0; x < Width; ++x)
        {
            auto PixelIndex = y * Width + x;

            double TotalSumX = 0;
            double TotalSumY = 0;
            double TotalSumZ = 0;
            double TotalSumW = 0;

            for (uint32_t i = 0; i < BucketsCount; ++i)
            {
                TotalSumX += UnestimatedImageData[i][PixelIndex * 4];
                TotalSumY += UnestimatedImageData[i][PixelIndex * 4 + 1];
                TotalSumZ += UnestimatedImageData[i][PixelIndex * 4 + 2];
                TotalSumW += UnestimatedImageData[i][PixelIndex * 4 + 3];
            }

            double GiniX = ((MaxValue[PixelIndex * 3] * TotalSumW) * 0.5 - TotalSumX) / ((MaxValue[PixelIndex * 3] * TotalSumW) * 0.5);
            double GiniY = ((MaxValue[PixelIndex * 3 + 1] * TotalSumW) * 0.5 - TotalSumY) / ((MaxValue[PixelIndex * 3 + 1] * TotalSumW) * 0.5);
            double GiniZ = ((MaxValue[PixelIndex * 3 + 2] * TotalSumW) * 0.5 - TotalSumZ) / ((MaxValue[PixelIndex * 3 + 2] * TotalSumW) * 0.5);

            bool bGini = false;

            if (GiniX > Threshold || GiniY > Threshold || GiniZ > Threshold)
            {
                bGini = true;
            }

            std::vector<double> ValuesX(BucketsCount);
            std::vector<double> ValuesY(BucketsCount);
            std::vector<double> ValuesZ(BucketsCount);

            for (uint32_t i = 0; i < BucketsCount; ++i)
            {
                ValuesX[i] = UnestimatedImageData[i][PixelIndex * 4] / UnestimatedImageData[i][PixelIndex * 4 + 3];
                ValuesY[i] = UnestimatedImageData[i][PixelIndex * 4 + 1] / UnestimatedImageData[i][PixelIndex * 4 + 3];
                ValuesZ[i] = UnestimatedImageData[i][PixelIndex * 4 + 2] / UnestimatedImageData[i][PixelIndex * 4 + 3];
            }

            if (bGini)
            {
                std::sort(ValuesX.begin(), ValuesX.end());
                std::sort(ValuesY.begin(), ValuesY.end());
                std::sort(ValuesZ.begin(), ValuesZ.end());

                EstimatedImageData[PixelIndex * 3] = float(LinearToGamma(ValuesX[BucketsCount / 2]));
                EstimatedImageData[PixelIndex * 3 + 1] = float(LinearToGamma(ValuesY[BucketsCount / 2]));
                EstimatedImageData[PixelIndex * 3 + 2] = float(LinearToGamma(ValuesZ[BucketsCount / 2]));
            }
            else
            {
                EstimatedImageData[PixelIndex * 3] = float(LinearToGamma(TotalSumX / TotalSumW));
                EstimatedImageData[PixelIndex * 3 + 1] = float(LinearToGamma(TotalSumY / TotalSumW));
                EstimatedImageData[PixelIndex * 3 + 2] = float(LinearToGamma(TotalSumZ / TotalSumW));
            }
        }
    }
}

void FEstimator::SaveAsEXR(const std::string& Name)
{
    const char* Err = nullptr;
    SaveEXR(EstimatedImageData.data(), Width, Height, 3, false, Name.c_str(), &Err);
}

void FEstimator::SaveAsBMP(const std::string& Name)
{
    std::vector<unsigned char> EstimatedUnsignedCharImageData(Width * Height * 3);
    static const FInterval Intensity(0, 0.999f);

    for (uint32_t i = 0; i < Width * Height; ++i)
    {
        EstimatedUnsignedCharImageData[i * 3] = 256 * Intensity.Clamp(LinearToGamma(EstimatedImageData[i * 3]));
        EstimatedUnsignedCharImageData[i * 3 + 1] = 256 * Intensity.Clamp(LinearToGamma(EstimatedImageData[i * 3 + 1]));
        EstimatedUnsignedCharImageData[i * 3 + 2] = 256 * Intensity.Clamp(LinearToGamma(EstimatedImageData[i * 3 + 2]));
    }

    stbi_write_bmp(Name.c_str(), Width, Height, 3, EstimatedUnsignedCharImageData.data());
}

double FEstimator::LinearToGamma(double Value) const
{
    if (Value > 0)
    {
        return sqrt(Value);
    }

    return 0;
}

#include "stb_image_write.h"
#include "tinyexr.h"

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
    ImagesData.resize(BucketsCount);
    MaxValue.resize(Width * Height * 3, 0.);

    for (uint32_t i = 0; i < BucketsCount; ++i)
    {
        ImagesData[i].resize(Width * Height * 4); /// Four components per pixel
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

    ImagesData[Counter[PixelIndex]][PixelIndex * 4] += Value.X;
    ImagesData[Counter[PixelIndex]][PixelIndex * 4 + 1] += Value.Y;
    ImagesData[Counter[PixelIndex]][PixelIndex * 4 + 2] += Value.Z;
    ImagesData[Counter[PixelIndex]][PixelIndex * 4 + 3] += 1.;

    Counter[PixelIndex] = (Counter[PixelIndex] + 1) % BucketsCount;
}

void FEstimator::SaveAsEXR(const std::string& Name)
{
    std::vector<float> ImageDataEstimated(Width * Height * 3);

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
                TotalSumX += ImagesData[i][PixelIndex * 4];
                TotalSumY += ImagesData[i][PixelIndex * 4 + 1];
                TotalSumZ += ImagesData[i][PixelIndex * 4 + 2];
                TotalSumW += ImagesData[i][PixelIndex * 4 + 3];
            }

            double GiniX = (MaxValue[PixelIndex * 3] * TotalSumW - TotalSumX) / (MaxValue[PixelIndex * 3] * TotalSumW);
            double GiniY = (MaxValue[PixelIndex * 3 + 1] * TotalSumW - TotalSumX) / (MaxValue[PixelIndex * 3 + 1] * TotalSumW);
            double GiniZ = (MaxValue[PixelIndex * 3 + 2] * TotalSumW - TotalSumX) / (MaxValue[PixelIndex * 3 + 2] * TotalSumW);

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
                ValuesX[i] = ImagesData[i][PixelIndex * 4] / ImagesData[i][PixelIndex * 4 + 3];
                ValuesX[i] = ImagesData[i][PixelIndex * 4 + 1] / ImagesData[i][PixelIndex * 4 + 3];
                ValuesX[i] = ImagesData[i][PixelIndex * 4 + 2] / ImagesData[i][PixelIndex * 4 + 3];
            }

            if (bGini)
            {
                std::sort(ValuesX.begin(), ValuesX.end());
                std::sort(ValuesY.begin(), ValuesY.end());
                std::sort(ValuesZ.begin(), ValuesZ.end());

                ImageDataEstimated[PixelIndex * 3] = float(ValuesX[BucketsCount / 2]);
                ImageDataEstimated[PixelIndex * 3 + 1] = float(ValuesX[BucketsCount / 2]);
                ImageDataEstimated[PixelIndex * 3 + 2] = float(ValuesX[BucketsCount / 2]);
            }
            else
            {
                ImageDataEstimated[PixelIndex * 3] = float(TotalSumX / TotalSumW);
                ImageDataEstimated[PixelIndex * 3 + 1] = float(TotalSumY / TotalSumW);
                ImageDataEstimated[PixelIndex * 3 + 2] = float(TotalSumZ / TotalSumW);
            }
        }
    }

    const char* Err = nullptr;
    SaveEXR(ImageDataEstimated.data(), Width, Height, 3, false, Name.c_str(), &Err);
}

void FEstimator::SaveAsBMP(const std::string& Name)
{
//    std::vector<unsigned char> EstimatedUnsignedCharImageData(ImageWidth * ImageHeight * 3);
//    static const FInterval Intensity(0, 0.999f);
//
//    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
//    {
//        double InverseAccumulated = 1 / ImageData[i * 4 + 3];
//        EstimatedUnsignedCharImageData[i * 3] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4] * InverseAccumulated));
//        EstimatedUnsignedCharImageData[i * 3 + 1] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 1] * InverseAccumulated));
//        EstimatedUnsignedCharImageData[i * 3 + 2] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 2] * InverseAccumulated));
//    }
//
//    stbi_write_bmp(Name.c_str(), ImageWidth, ImageHeight, 3, EstimatedUnsignedCharImageData.data());
}

double FEstimator::LinearToGamma(double Value) const
{
    if (Value > 0)
    {
        return sqrt(Value);
    }

    return 0;
}

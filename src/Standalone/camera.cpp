#include "stb_image_write.h"
#include "tinyexr.h"

#include "common_defines.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <chrono>

FColor3 FCamera::RayColor(const FRay &Ray, uint32_t Depth, const FHittable &World)
{
    if (Depth <= 0)
    {
        return {};
    }

    FHitRecord HitRecord;

    if (World.Hit(Ray, {0.0001f, INFINITY}, HitRecord))
    {
        FRay Scattered;
        FColor3 Attenuation;

        if (HitRecord.Material->Scatter(Ray, HitRecord, Attenuation, Scattered))
        {
            FColor3 Color = RayColor(Scattered, Depth - 1, World);
            return Attenuation * Color;
        }

        return {0, 0, 0};
    }

    FVector3 UnitDirection = Ray.GetDirection().GetNormalized();
    double A = 0.5 * (UnitDirection.Y + 1);
    return FColor3 (1, 1, 1) * (1 - A) + FColor3(0.5, 0.7, 1) * A;
}

void FCamera::Initialize()
{
    ImageHeight = ImageWidth / AspectRatio;
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    ImageData = std::vector<double>(ImageWidth * ImageHeight * 4);

    CameraCenter = LookFrom;

    double Theta = DegreesToRadians(VFOV);
    double H = tan(Theta * 0.5);
    double ViewportHeight = 2 * H * FocusDistance;
    double ViewportWidth = ViewportHeight * (double(ImageWidth) / double(ImageHeight));

    W = (LookFrom - LookAt).GetNormalized();
    U = Cross(Up, W).GetNormalized();
    V = Cross(W, U);

    FVector3 ViewportU = ViewportWidth * U;
    FVector3 ViewportV = -ViewportHeight * V;
    PixelDeltaU = ViewportU / double(ImageWidth);
    PixelDeltaV = ViewportV / double(ImageHeight);

    Pixel00 = CameraCenter - (FocusDistance * W) - ViewportU * 0.5 - ViewportV * 0.5;;

    double DefocusRadius = FocusDistance * tan(DegreesToRadians(DefocusAngle * 0.5f));
    DefocusDiscU = U * DefocusRadius;
    DefocusDiscV = V * DefocusRadius;
}

FRay FCamera::GetRay(uint32_t X, uint32_t Y)
{
    auto PixelUpperLeft = Pixel00 + (PixelDeltaU * X) + (PixelDeltaV * Y);
    auto [JitterX, JitterY] = RNG2();
    auto JitteredPixel = PixelUpperLeft + (PixelDeltaU * JitterX) + (PixelDeltaV * JitterY);
    auto RayOrigin = CameraCenter;

    if (DefocusAngle > 0)
    {
        auto PointOnDisc = RandomInUnitDisc();
        RayOrigin = CameraCenter + (PointOnDisc.X * DefocusDiscU) + (PointOnDisc.Y * DefocusDiscV);
    }

    auto RayDirection = JitteredPixel - RayOrigin; /// Not normalized
    FRay Ray(RayOrigin, RayDirection, RandomDouble());
    return Ray;
}

void FCamera::Render(const FHittable &World)
{
    Initialize();

    uint32_t LineCounter = ImageHeight;
    std::mutex Mutex;

    auto ScanLines = [&](std::vector<uint32_t> Lines)
    {
        for (auto Line : Lines)
        {
            {
                std::lock_guard<std::mutex> Lock(Mutex);
                std::cout << "Scanline remaining: " << (LineCounter) << std::endl;
                LineCounter--;
            }

            for(uint32_t j = 0; j < ImageWidth; ++j)
            {
                for (uint32_t k = 0; k < IterationsPerPixel; ++k)
                {
                    auto Ray = GetRay(j, Line);
                    FColor3 PixelColor = RayColor(Ray, MaxDepth, World);
                    WriteColor(PixelColor, Line * ImageWidth + j);
                }
            }
        }
    };

    uint32_t ThreadsCount = std::thread::hardware_concurrency() / 4;
    ThreadsCount = ThreadsCount > 0 ? ThreadsCount : 1;

    std::vector<std::vector<uint32_t>> LinesByThread(ThreadsCount);

    for (uint32_t i = 0; i < ImageHeight; ++i)
    {
        LinesByThread[i % ThreadsCount].push_back(i);
    }

    std::vector<std::thread> Threads;

    std::chrono::time_point<std::chrono::high_resolution_clock> Start = std::chrono::steady_clock::now();
    for (auto Entry : LinesByThread)
    {
        Threads.emplace_back(ScanLines, Entry);
    }

    for (auto& Thread : Threads)
    {
        Thread.join();
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> End = std::chrono::steady_clock::now();
    std::chrono::duration<double> Elapsed = End - Start;

    std::cout << "Rendering finished. Time: " <<  Elapsed.count() << std::endl;
}

void FCamera::WriteColor(const FColor3& PixelColor, uint32_t PixelIndex)
{
    ImageData[PixelIndex * 4] += PixelColor.X;
    ImageData[PixelIndex * 4 + 1] += PixelColor.Y;
    ImageData[PixelIndex * 4 + 2] += PixelColor.Z;
    ImageData[PixelIndex * 4 + 3] += 1;
}

double FCamera::LinearToGamma(double Value) const
{
    if (Value > 0)
    {
        return sqrt(Value);
    }

    return 0;
}

void FCamera::SaveAsEXR(const std::string &Name)
{
    std::vector<float> ImageDataEstimated(ImageWidth * ImageHeight * 3);
    static const FInterval Intensity(0, 0.999f);

    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
    {
        double InverseAccumulated = 1 / ImageData[i * 4 + 3];
        ImageDataEstimated[i * 3] = Intensity.Clamp(LinearToGamma(ImageData[i * 4] * InverseAccumulated));
        ImageDataEstimated[i * 3 + 1] = Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 1] * InverseAccumulated));
        ImageDataEstimated[i * 3 + 2] = Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 2] * InverseAccumulated));
    }

    const char* Err = nullptr;
    SaveEXR(ImageDataEstimated.data(), ImageWidth, ImageHeight, 3, false, Name.c_str(), &Err);
}

void FCamera::SaveAsBMP(const std::string &Name)
{
    std::vector<unsigned char> EstimatedUnsignedCharImageData(ImageWidth * ImageHeight * 3);
    static const FInterval Intensity(0, 0.999f);

    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
    {
        double InverseAccumulated = 1 / ImageData[i * 4 + 3];
        EstimatedUnsignedCharImageData[i * 3] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4] * InverseAccumulated));
        EstimatedUnsignedCharImageData[i * 3 + 1] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 1] * InverseAccumulated));
        EstimatedUnsignedCharImageData[i * 3 + 2] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 2] * InverseAccumulated));
    }

    stbi_write_bmp(Name.c_str(), ImageWidth, ImageHeight, 3, EstimatedUnsignedCharImageData.data());
}
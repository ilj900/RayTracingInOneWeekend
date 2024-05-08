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

    if (!World.Hit(Ray, FInterval(0.0001, Infinity), HitRecord))
    {
        return Background;
    }

    FRay Scattered;
    FColor3 Attenuation;
    double PDF;
    FColor3 ColorFromEmission = HitRecord.Material->Emit(Ray, HitRecord, HitRecord.U, HitRecord.V, HitRecord.Position);

    if (!HitRecord.Material->Scatter(Ray, HitRecord, Attenuation, Scattered, PDF))
    {
        return ColorFromEmission;
    }

    auto OnLight = FPoint3(RandomDouble(213, 343), 554, RandomDouble(227, 332));
    auto ToLight = OnLight - HitRecord.Position;
    auto Distance2 = ToLight.Length2();
    ToLight.Normalize();

    if (Dot(ToLight, HitRecord.Normal) < 0.)
    {
        return ColorFromEmission;
    }

    double LightArea = (343 - 213) * (332 - 227);
    auto LightCos = fabs(ToLight.Y);
    if (LightCos < 0.000001)
    {
        return ColorFromEmission;
    }

    PDF = Distance2 / (LightCos * LightArea);
    Scattered = FRay(HitRecord.Position, ToLight, Ray.GetTime());

    double ScatteringPDF = HitRecord.Material->ScatteringPDF(Ray, HitRecord, Scattered);

    FColor3 ColorFromScatter = Attenuation * ScatteringPDF * RayColor(Scattered, Depth - 1, World) / PDF;

    return ColorFromEmission + ColorFromScatter;
}

void FCamera::Initialize()
{
    ImageHeight = ImageWidth / AspectRatio;
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    SqrtSPP = int(sqrt(SamplesPerPixel));
    RecipSqrtSPP = 1. / SqrtSPP;
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

    Pixel00 = CameraCenter - (FocusDistance * W) - ViewportU * 0.5 - ViewportV * 0.5;

    double DefocusRadius = FocusDistance * tan(DegreesToRadians(DefocusAngle * 0.5f));
    DefocusDiscU = U * DefocusRadius;
    DefocusDiscV = V * DefocusRadius;
}

FRay FCamera::GetRay(uint32_t X, uint32_t Y, uint32_t SX, uint32_t SY)
{
    auto PX = ((SX + RandomDouble()) * RecipSqrtSPP) - 0.5;
    auto PY = ((SY + RandomDouble()) * RecipSqrtSPP) - 0.5;
    FVector3 Offset = {PX, PY, 0};

    auto PixelSample = Pixel00 + ((X + Offset.X) * PixelDeltaU) + ((Y + Offset.Y) * PixelDeltaV);


    auto RayOrigin = CameraCenter;

    if (DefocusAngle > 0)
    {
        auto PointOnDisc = RandomInUnitDisc();
        RayOrigin = CameraCenter + (PointOnDisc.X * DefocusDiscU) + (PointOnDisc.Y * DefocusDiscV);
    }

    auto RayDirection = PixelSample - RayOrigin; /// Not normalized
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
                for (uint32_t si = 0; si < SqrtSPP; si++)
                {
                    for (uint32_t sj = 0; sj < SqrtSPP; sj++)
                    {
                        auto Ray = GetRay(j, Line, si, sj);
                        FColor3 PixelColor = RayColor(Ray, MaxDepth, World);
                        WriteColor(PixelColor, Line * ImageWidth + j);
                    }
                }
            }
        }
    };

    uint32_t ThreadsCount = std::thread::hardware_concurrency();

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
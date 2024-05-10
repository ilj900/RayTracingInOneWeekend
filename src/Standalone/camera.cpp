#include "common_defines.h"
#include "camera.h"
#include "material.h"
#include "pdf.h"
#include "hittable_list.h"

#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <chrono>

FColor3 FCamera::RayColor(const FRay &Ray, uint32_t Depth, const FHittable &World, const FHittable& Lights)
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

    FScatterRecord ScatterRecord;
    FColor3 ColorFromEmission = HitRecord.Material->Emit(Ray, HitRecord, HitRecord.U, HitRecord.V, HitRecord.Position);

    if (!HitRecord.Material->Scatter(Ray, HitRecord, ScatterRecord))
    {
        return ColorFromEmission;
    }

    if (ScatterRecord.bSkipPDF)
    {
        return ScatterRecord.Attenuation * RayColor(ScatterRecord.SkipPDFRay, Depth - 1, World, Lights);
    }

    auto LightPtr = std::make_shared<FHittablePDF>(Lights, HitRecord.Position);
    FMixturePDF P(LightPtr, ScatterRecord.PDFPtr);

    FRay Scattered = FRay(HitRecord.Position, P.Generate(), Ray.GetTime());
    auto PDFValue = P.Value(Scattered.GetDirection());

    double ScatteringPDF = HitRecord.Material->ScatteringPDF(Ray, HitRecord, Scattered);

    FVector3 SampleColor = RayColor(Scattered, Depth - 1, World, Lights);
    FColor3 ColorFromScatter = (ScatterRecord.Attenuation * ScatteringPDF * SampleColor) / PDFValue;

    return ColorFromEmission + ColorFromScatter;
}

void FCamera::Initialize()
{
    ImageHeight = ImageWidth / AspectRatio;
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    SqrtSPP = int(sqrt(SamplesPerPixel));
    RecipSqrtSPP = 1. / SqrtSPP;

    Estimator = std::make_shared<FEstimator>(ImageWidth, ImageHeight, 9, 0.25);

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

void FCamera::Render(const FHittable &World, const FHittable& Lights)
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
                        FColor3 PixelColor = RayColor(Ray, MaxDepth, World, Lights);
                        Estimator->Store(j, Line, PixelColor);
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

void FCamera::SaveAsEXR(const std::string& Name)
{
    Estimator->DoEstimate();
    Estimator->SaveAsEXR(Name);
}

void FCamera::SaveAsBMP(const std::string& Name)
{
    Estimator->DoEstimate();
    Estimator->SaveAsBMP(Name);
}
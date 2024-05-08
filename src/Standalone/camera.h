#pragma once

#include "color3.h"
#include "hittable.h"
#include "rng.h"

#include <vector>

class FCamera
{
public:
    void Render(const FHittable& World, const FHittable& Lights);
    void SaveAsEXR(const std::string& Name);
    void SaveAsBMP(const std::string& Name);

    double AspectRatio = 16.0 / 9.0;
    uint32_t ImageWidth = 1920;
    uint32_t SamplesPerPixel = 10;
    uint32_t MaxDepth = 10;
    double VFOV = 90;
    FPoint3 LookFrom = {0, 0, 0};
    FPoint3 LookAt = {0, 0, -1};
    FVector3 Up = {0, 1, 0};
    double DefocusAngle = 0;
    double FocusDistance = 10;
    FColor3 Background;

private:
    void Initialize();
    FRay GetRay(uint32_t X, uint32_t Y, uint32_t SX, uint32_t SY);
    FColor3 RayColor(const FRay& Ray, uint32_t Depth, const FHittable& World, const FHittable& Lights);
    void WriteColor(const FColor3& PixelColor, uint32_t PixelIndex);
    double LinearToGamma(double Value) const;

    uint32_t ImageHeight;
    int SqrtSPP;
    double RecipSqrtSPP;
    FPoint3 CameraCenter;
    FVector3 Pixel00;
    FVector3 PixelDeltaU;
    FVector3 PixelDeltaV;
    FVector3 U;
    FVector3 V;
    FVector3 W;
    FVector3 DefocusDiscU;
    FVector3 DefocusDiscV;

    std::vector<double> ImageData;

    RNG2D RNG2;
    RNG3D RNG3;
};

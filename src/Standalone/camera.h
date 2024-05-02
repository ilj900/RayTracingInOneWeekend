#ifndef CAMERA_H
#define CAMERA_H

#include "color3.h"
#include "hittable.h"
#include "rng.h"

#include <vector>

class FCamera
{
public:
    void Render(const FHittable& World);
    void SaveAsEXR(const std::string& Name);
    void SaveAsBMP(const std::string& Name);

    float AspectRatio = 16.f / 9.f;
    uint32_t ImageWidth = 1920;
    uint32_t IterationsPerPixel = 10;

private:
    void Initialize();
    FRay GetRay(uint32_t X, uint32_t Y);
    FColor3 RayColor(const FRay& Ray, const FHittable& World) const;
    void WriteColor(const FColor3& PixelColor, uint32_t PixelIndex);

    uint32_t ImageHeight;
    FPoint3 CameraCenter;
    FVector3 Pixel00;
    FVector3 PixelDeltaU;
    FVector3 PixelDeltaV;

    std::vector<float> ImageData;

    RNG1D RNG;
};

#endif // CAMERA_H
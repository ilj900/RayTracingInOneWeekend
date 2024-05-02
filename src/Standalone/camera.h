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
    uint32_t MaxDepth = 10;

private:
    void Initialize();
    FRay GetRay(uint32_t X, uint32_t Y);
    FColor3 RayColor(const FRay& Ray, uint32_t Depth, const FHittable& World);
    void WriteColor(const FColor3& PixelColor, uint32_t PixelIndex);
    FVector3 RandomVector3();
    FVector3 RandomVector3(float Min, float Max);
    FVector3 RandomVectorInUnitSphere();
    FVector3 RandomUnitVector();
    FVector3 RandomUnitVectorOnHemisphere(const FVector3& Normal);
    float LinearToGamma(float Value) const;

    uint32_t ImageHeight;
    FPoint3 CameraCenter;
    FVector3 Pixel00;
    FVector3 PixelDeltaU;
    FVector3 PixelDeltaV;

    std::vector<float> ImageData;

    RNG2D RNG2;
    RNG3D RNG3;
};

#endif // CAMERA_H
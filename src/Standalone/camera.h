#ifndef CAMERA_H
#define CAMERA_H

#include "color3.h"
#include "hittable.h"

class FCamera
{
public:
    void Render(const FHittable& World);
    void SaveAsEXR(const std::string& Name);
    void SaveAsBMP(const std::string& Name);

    float AspectRatio = 16.f / 9.f;
    uint32_t ImageWidth = 1920;

private:
    void Initialize();
    FColor3 RayColor(const FRay& Ray, const FHittable& World) const;

    uint32_t ImageHeight;
    FPoint3 CameraCenter;
    FVector3 Pixel00;
    FVector3 PixelDeltaU;
    FVector3 PixelDeltaV;

    std::vector<float> FloatImageData;
    std::vector<unsigned char> UnsignedCharImageData;
};

#endif // CAMERA_H
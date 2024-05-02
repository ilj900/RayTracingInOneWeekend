#include "stb_image_write.h"
#include "tinyexr.h"

#include "color3.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

FColor3 RayColor(const FRay& Ray, const FHittable& World)
{
    FHitRecord HitRecord;

    if (World.Hit(Ray, 0, INFINITY, HitRecord))
    {
        return 0.5f * (HitRecord.Normal + FColor3(1, 1, 1));
    }

    FVector3 UnitDirection = Ray.GetDirection().GetNormalized();
    float A = 0.5f * (UnitDirection.Y + 1.f);
    return FColor3 (1, 1, 1) * (1.f - A) + FColor3(0.5, 0.7, 1) * A;
}

int main()
{
    /// Image
    float AspectRatio = 16.f / 9.f;
    uint32_t ImageWidth = 1920;
    uint32_t ImageHeight = ImageWidth / AspectRatio;
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    std::vector<float> FloatImageData;
    FloatImageData.reserve(ImageWidth * ImageHeight * 3);
    std::vector<unsigned char> UnsignedCharImageData;
    UnsignedCharImageData.reserve(ImageWidth * ImageHeight * 3);

    FHittableList World;

    World.Add(std::make_shared<FSphere>(FVector3{0, 0, -1}, 0.5f));
    World.Add(std::make_shared<FSphere>(FVector3{0, -100.5, -1}, 100.f));

    /// Camera
    float FocalLength = 1.0;
    float ViewportHeight = 2.f;
    float ViewportWidth = ViewportHeight * (float(ImageWidth) / float(ImageHeight));
    FPoint3 CameraCenter{};
    FVector3 ViewportU{ViewportWidth, 0, 0};
    FVector3 ViewportV{0, -ViewportHeight, 0};
    auto PixelDeltaU = ViewportU / float(ImageWidth);
    auto PixelDeltaV = ViewportV / float(ImageHeight);
    FPoint3 ViewportUpperLeft = CameraCenter - FVector3{0, 0, FocalLength} - (ViewportU * 0.5f) - (ViewportV * 0.5f);
    auto Pixel00 = ViewportUpperLeft + ((PixelDeltaU + PixelDeltaV) * 0.5);

    for (uint32_t i = 0; i < ImageHeight; ++i)
    {
        std::cout << "Scanline remaining: " << (ImageHeight - i) << std::endl;

        for(uint32_t j = 0; j < ImageWidth; ++j)
        {
            auto PixelCenter = Pixel00 + (PixelDeltaU * j) + (PixelDeltaV * i);
            auto RayDirection = PixelCenter - CameraCenter; /// Not normalized
            FRay Ray(CameraCenter, RayDirection);
            FColor3 PixelColor = RayColor(Ray, World);

            WriteColor(FloatImageData, PixelColor);
            WriteColor(UnsignedCharImageData, PixelColor);
        }
    }

    std::cout << "Rendering finished." << std::endl;

    const char* Err = NULL;
    SaveEXR(FloatImageData.data(), ImageWidth, ImageHeight, 3, false, "Result.exr", &Err);

    stbi_write_bmp("Result.bmp", ImageWidth, ImageHeight, 3, UnsignedCharImageData.data());
}
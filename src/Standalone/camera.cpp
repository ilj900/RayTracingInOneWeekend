#include "stb_image_write.h"
#include "tinyexr.h"

#include "camera.h"

#include <iostream>

FColor3 FCamera::RayColor(const FRay &Ray, const FHittable &World) const
{
    FHitRecord HitRecord;

    if (World.Hit(Ray, {0, INFINITY}, HitRecord))
    {
        return 0.5f * (HitRecord.Normal + FColor3(1, 1, 1));
    }

    FVector3 UnitDirection = Ray.GetDirection().GetNormalized();
    float A = 0.5f * (UnitDirection.Y + 1.f);
    return FColor3 (1, 1, 1) * (1.f - A) + FColor3(0.5, 0.7, 1) * A;
}

void FCamera::Initialize()
{
    ImageHeight = ImageWidth / AspectRatio;
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    ImageData = std::vector<float>(ImageWidth * ImageHeight * 4);

    CameraCenter = {0, 0, 0};
    float FocalLength = 1.0;
    float ViewportHeight = 2.f;
    float ViewportWidth = ViewportHeight * (float(ImageWidth) / float(ImageHeight));
    FVector3 ViewportU{ViewportWidth, 0, 0};
    FVector3 ViewportV{0, -ViewportHeight, 0};
    PixelDeltaU = ViewportU / float(ImageWidth);
    PixelDeltaV = ViewportV / float(ImageHeight);

    Pixel00 = CameraCenter - FVector3{0, 0, FocalLength} - (ViewportU * 0.5f) - (ViewportV * 0.5f);
}

FRay FCamera::GetRay(uint32_t X, uint32_t Y)
{
    auto PixelUpperLeft = Pixel00 + (PixelDeltaU * X) + (PixelDeltaV * Y);
    auto [JitterX, JitterY] = RNG();
    auto JitteredPixel = PixelUpperLeft + (PixelDeltaU * JitterX) + (PixelDeltaV * JitterY);
    auto RayDirection = JitteredPixel - CameraCenter; /// Not normalized
    FRay Ray(CameraCenter, RayDirection);
    return Ray;
}

void FCamera::Render(const FHittable &World)
{
    Initialize();

    for (uint32_t i = 0; i < ImageHeight; ++i)
    {
        std::cout << "Scanline remaining: " << (ImageHeight - i) << std::endl;

        for(uint32_t j = 0; j < ImageWidth; ++j)
        {
            for (uint32_t k = 0; k < IterationsPerPixel; ++k)
            {
                auto Ray = GetRay(j, i);
                FColor3 PixelColor = RayColor(Ray, World);
                WriteColor(PixelColor, i * ImageWidth + j);
            }
        }
    }

    std::cout << "Rendering finished." << std::endl;
}

void FCamera::WriteColor(const FColor3& PixelColor, uint32_t PixelIndex)
{
    ImageData[PixelIndex * 4] += PixelColor.X;
    ImageData[PixelIndex * 4 + 1] += PixelColor.Y;
    ImageData[PixelIndex * 4 + 2] += PixelColor.Z;
    ImageData[PixelIndex * 4 + 3] += 1.f;
}

void FCamera::SaveAsEXR(const std::string &Name)
{
    std::vector<float> ImageDataEstimated(ImageWidth * ImageHeight * 3);
    static const FInterval Intensity(0.f, 0.999f);

    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
    {
        float InverseAccumulated = 1.f / ImageData[i * 4 + 3];
        ImageDataEstimated[i * 3] = Intensity.Clamp(ImageData[i * 4] * InverseAccumulated);
        ImageDataEstimated[i * 3 + 1] = Intensity.Clamp(ImageData[i * 4 + 1] * InverseAccumulated);
        ImageDataEstimated[i * 3 + 2] = Intensity.Clamp(ImageData[i * 4 + 2] * InverseAccumulated);
    }

    const char* Err = nullptr;
    SaveEXR(ImageDataEstimated.data(), ImageWidth, ImageHeight, 3, false, Name.c_str(), &Err);
}

void FCamera::SaveAsBMP(const std::string &Name)
{
    std::vector<unsigned char> EstimatedUnsignedCharImageData(ImageWidth * ImageHeight * 3);
    static const FInterval Intensity(0.f, 0.999f);

    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
    {
        float InverseAccumulated = 1.f / ImageData[i * 4 + 3];
        EstimatedUnsignedCharImageData[i * 3] = 256 * Intensity.Clamp(ImageData[i * 4] * InverseAccumulated);
        EstimatedUnsignedCharImageData[i * 3 + 1] = 256 * Intensity.Clamp(ImageData[i * 4 + 1] * InverseAccumulated);
        EstimatedUnsignedCharImageData[i * 3 + 2] = 256 * Intensity.Clamp(ImageData[i * 4 + 2] * InverseAccumulated);
    }

    stbi_write_bmp(Name.c_str(), ImageWidth, ImageHeight, 3, EstimatedUnsignedCharImageData.data());
}
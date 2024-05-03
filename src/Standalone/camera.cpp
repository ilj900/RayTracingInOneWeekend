#include "stb_image_write.h"
#include "tinyexr.h"

#include "common_defines.h"
#include "camera.h"
#include "material.h"

#include <iostream>

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
    float A = 0.5f * (UnitDirection.Y + 1.f);
    return FColor3 (1, 1, 1) * (1.f - A) + FColor3(0.5, 0.7, 1) * A;
}

void FCamera::Initialize()
{
    ImageHeight = ImageWidth / AspectRatio;
    ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;
    ImageData = std::vector<float>(ImageWidth * ImageHeight * 4);

    CameraCenter = LookFrom;
    
    float FocalLength = (LookFrom - LookAt).Length();
    float Theta = DegreesToRadians(VFOV);
    float H = tan(Theta * 0.5f);
    float ViewportHeight = 2.f * H * FocalLength;
    float ViewportWidth = ViewportHeight * (float(ImageWidth) / float(ImageHeight));

    W = (LookFrom - LookAt).GetNormalized();
    U = Cross(Up, W).GetNormalized();
    V = Cross(W, U);

    FVector3 ViewportU = ViewportWidth * U;
    FVector3 ViewportV = -ViewportHeight * V;
    PixelDeltaU = ViewportU / float(ImageWidth);
    PixelDeltaV = ViewportV / float(ImageHeight);

    Pixel00 = CameraCenter - (FocalLength * W) - ViewportU * 0.5 - ViewportV * 0.5;;
}

FRay FCamera::GetRay(uint32_t X, uint32_t Y)
{
    auto PixelUpperLeft = Pixel00 + (PixelDeltaU * X) + (PixelDeltaV * Y);
    auto [JitterX, JitterY] = RNG2();
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
            if (i == 500 && j == 500)
            {
                int dd = 0;
                dd++;
            }
            for (uint32_t k = 0; k < IterationsPerPixel; ++k)
            {
                auto Ray = GetRay(j, i);
                FColor3 PixelColor = RayColor(Ray, MaxDepth, World);
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

float FCamera::LinearToGamma(float Value) const
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
    static const FInterval Intensity(0.f, 0.999f);

    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
    {
        float InverseAccumulated = 1.f / ImageData[i * 4 + 3];
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
    static const FInterval Intensity(0.f, 0.999f);

    for (uint32_t i = 0; i < ImageWidth * ImageHeight; ++i)
    {
        float InverseAccumulated = 1.f / ImageData[i * 4 + 3];
        EstimatedUnsignedCharImageData[i * 3] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4] * InverseAccumulated));
        EstimatedUnsignedCharImageData[i * 3 + 1] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 1] * InverseAccumulated));
        EstimatedUnsignedCharImageData[i * 3 + 2] = 256 * Intensity.Clamp(LinearToGamma(ImageData[i * 4 + 2] * InverseAccumulated));
    }

    stbi_write_bmp(Name.c_str(), ImageWidth, ImageHeight, 3, EstimatedUnsignedCharImageData.data());
}
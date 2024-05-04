#pragma once

#include "color3.h"

#include "texture.h"

class FRay;
class FHitRecord;

class FMaterial
{
public:
    virtual ~FMaterial() = default;

    virtual bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const;
};

class FLambertian : public FMaterial
{
public:
    FLambertian(const FColor3& AlbedoIn);
    FLambertian(std::shared_ptr<FTexture> TextureIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const override;

private:
    std::shared_ptr<FTexture> Texture;
};

class FMetal : public FMaterial
{
public:
    FMetal(const FColor3& AlbedoIn, double FuzzIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const override;

private:
    FColor3 Albedo;
    double Fuzz;
};

class FDielectric : public FMaterial
{
public:
    FDielectric(double RefractionIndexIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const override;

private:
    double RefractionIndex;

    static double Reflectance(double Cosine, double RefractionIndex);
};

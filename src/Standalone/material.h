#ifndef MATERIAL_H
#define MATERIAL_H

#include "color3.h"

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
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const override;

private:
    FColor3 Albedo;
};

class FMetal : public FMaterial
{
public:
    FMetal(const FColor3& AlbedoIn, float FuzzIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const override;

private:
    FColor3 Albedo;
    float Fuzz;
};

class FDielectric : public FMaterial
{
public:
    FDielectric(float RefractionIndexIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const override;

private:
    float RefractionIndex;

    static float Reflectance(float Cosine, float RefractionIndex);
};

#endif // MATERIAL_H
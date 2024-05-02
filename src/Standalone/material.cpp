#include "hittable.h"
#include "rng.h"

#include "material.h"

bool FMaterial::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const
{
    return false;
}

FLambertian::FLambertian(const FColor3& AlbedoIn) : Albedo(AlbedoIn) {};

bool FLambertian::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const
{
    auto ScatterDirection = HitRecord.Normal + RandomUnitVector();

    if (ScatterDirection.NearZero())
    {
        ScatterDirection = HitRecord.Normal;
    }

    Scattered = FRay(HitRecord.Position, ScatterDirection);
    Attenuation = Albedo;
    return true;
}

FMetal::FMetal(const FColor3& AlbedoIn, float FuzzIn) : Albedo(AlbedoIn), Fuzz(FuzzIn) {};

bool FMetal::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const
{
    FVector3 Reflected = Reflect(Ray.GetDirection(), HitRecord.Normal);
    Reflected = Reflected.GetNormalized() + (Fuzz * RandomUnitVector());
    Scattered = FRay(HitRecord.Position, Reflected);
    Attenuation = Albedo;
    return Dot(Scattered.GetDirection(), HitRecord.Normal) > 0.f;
}
#include "hittable.h"
#include "rng.h"

#include "material.h"

FColor3 FMaterial::Emit(double U, double V, const FPoint3& Point) const
{
    return FColor3(0, 0, 0);
}

bool FMaterial::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const
{
    return false;
};

FLambertian::FLambertian(const FColor3& AlbedoIn) : Texture(std::make_shared<FSolidColor>(AlbedoIn)) {};

FLambertian::FLambertian(std::shared_ptr<FTexture> TextureIn) : Texture(TextureIn) {};

bool FLambertian::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const
{
    auto ScatterDirection = HitRecord.Normal + RandomUnitVector();
    ScatterDirection.Normalize();

    if (ScatterDirection.NearZero())
    {
        ScatterDirection = HitRecord.Normal;
    }

    Scattered = FRay(HitRecord.Position, ScatterDirection, Ray.GetTime());
    Attenuation = Texture->Value(HitRecord.U, HitRecord.V, HitRecord.Position);

    return true;
};

FMetal::FMetal(const FColor3& AlbedoIn, double FuzzIn) : Albedo(AlbedoIn), Fuzz(FuzzIn) {};

bool FMetal::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FColor3& Attenuation, FRay& Scattered) const
{
    FVector3 Reflected = Reflect(Ray.GetDirection(), HitRecord.Normal);
    Reflected = Reflected.GetNormalized() + (Fuzz * RandomUnitVector());
    Scattered = FRay(HitRecord.Position, Reflected, Ray.GetTime());
    Attenuation = Albedo;

    return Dot(Scattered.GetDirection(), HitRecord.Normal) > 0;
};

FDielectric::FDielectric(double RefractionIndexIn) : RefractionIndex(RefractionIndexIn) {};

bool FDielectric::Scatter(const FRay &Ray, const FHitRecord &HitRecord, FColor3 &Attenuation, FRay &Scattered) const
{
    Attenuation = {1, 1, 1};
    double RelativeRefractionIndex = HitRecord.bFrontFace ? (1 / RefractionIndex) : RefractionIndex;
    auto NormalizedRayDirection = Ray.GetDirection().GetNormalized();

    double CosTheta = fmin(Dot(-NormalizedRayDirection, HitRecord.Normal), 1);
    double SinTheta = sqrt(1 - CosTheta * CosTheta);

    bool CannotRefract = (RelativeRefractionIndex * SinTheta) > 1;
    FVector3 Direction;

    if (CannotRefract || (Reflectance(CosTheta, RelativeRefractionIndex) > RandomDouble()))
    {
        Direction = Reflect(NormalizedRayDirection, HitRecord.Normal);
    }
    else
    {
        Direction = Refract(NormalizedRayDirection, HitRecord.Normal, RelativeRefractionIndex);
    }

    Scattered = {HitRecord.Position, Direction, Ray.GetTime()};

    return true;
};

double FDielectric::Reflectance(double Cosine, double RefractionIndex)
{
    double R0 = (1 - RefractionIndex) / (1 + RefractionIndex);
    R0 = R0 * R0;
    return R0 + (1 - R0) * pow(1 - Cosine, 5);
};

FDiffuseLight::FDiffuseLight(std::shared_ptr<FTexture> TextureIn) : Texture(TextureIn) {};

FDiffuseLight::FDiffuseLight(const FColor3& EmissionColor) : Texture(std::make_shared<FSolidColor>(EmissionColor)) {};

FColor3 FDiffuseLight::Emit(double U, double V, const FPoint3& Point) const
{
    return Texture->Value(U, V, Point);
}



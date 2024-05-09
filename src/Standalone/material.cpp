#include "common_defines.h"
#include "hittable.h"
#include "onb.h"
#include "rng.h"

#include "material.h"

FColor3 FMaterial::Emit(const FRay& Ray, const FHitRecord& HitRecord, double U, double V, const FPoint3& Point) const
{
    return FColor3(0, 0, 0);
}

bool FMaterial::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const
{
    return false;
};

double FMaterial::ScatteringPDF(const FRay& Ray, const FHitRecord& HitRecord, const FRay& Scattered) const
{
    return 0;
}

FLambertian::FLambertian(const FColor3& AlbedoIn) : Texture(std::make_shared<FSolidColor>(AlbedoIn)) {};

FLambertian::FLambertian(std::shared_ptr<FTexture> TextureIn) : Texture(TextureIn) {};

bool FLambertian::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const
{
    ScatterRecord.Attenuation = Texture->Value(HitRecord.U, HitRecord.V, HitRecord.Position);
    ScatterRecord.PDFPtr = std::make_shared<FCosinePDF>(HitRecord.Normal);
    ScatterRecord.bSkipPDF = false;
    return true;
};

double FLambertian::ScatteringPDF(const FRay& Ray, const FHitRecord& HitRecord, const FRay& Scattered) const
{
    return M_PI_INV * 0.5;
}

FMetal::FMetal(const FColor3& AlbedoIn, double FuzzIn) : Albedo(AlbedoIn), Fuzz(FuzzIn) {};

bool FMetal::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const
{
    FVector3 Reflected = Reflect(Ray.GetDirection(), HitRecord.Normal);
    Reflected = Reflected.GetNormalized() + (Fuzz * RandomUnitVector());

    ScatterRecord.Attenuation = Albedo;
    ScatterRecord.PDFPtr = nullptr;
    ScatterRecord.bSkipPDF = true;
    ScatterRecord.SkipPDFRay = FRay(HitRecord.Position, Reflected, Ray.GetTime());

    return true;
};

FDielectric::FDielectric(double RefractionIndexIn) : RefractionIndex(RefractionIndexIn) {};

bool FDielectric::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const
{
    ScatterRecord.Attenuation = {1, 1, 1};
    ScatterRecord.PDFPtr = nullptr;
    ScatterRecord.bSkipPDF = true;
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

    ScatterRecord.SkipPDFRay = {HitRecord.Position, Direction, Ray.GetTime()};

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

FColor3 FDiffuseLight::Emit(const FRay& Ray, const FHitRecord& HitRecord, double U, double V, const FPoint3& Point) const
{
    if (!HitRecord.bFrontFace)
    {
        return {};
    }
    return Texture->Value(U, V, Point);
}

FIsotropic::FIsotropic(const FColor3& Albedo) : Texture(std::make_shared<FSolidColor>(Albedo)) {};

FIsotropic::FIsotropic(std::shared_ptr<FTexture> TextureIn) : Texture(TextureIn) {};

bool FIsotropic::Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const
{
    ScatterRecord.Attenuation = Texture->Value(HitRecord.U, HitRecord.V, HitRecord.Position);
    ScatterRecord.PDFPtr = std::make_shared<FSpherePDF>();
    ScatterRecord.bSkipPDF = false;
    return true;
}

double FIsotropic::ScatteringPDF(const FRay& Ray, const FHitRecord& HitRecord, const FRay& Scattered) const
{
    return M_PI_INV * 0.25;
}



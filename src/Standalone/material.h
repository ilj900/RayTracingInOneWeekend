#pragma once

#include "pdf.h"
#include "color3.h"

#include "texture.h"

class FRay;
class FHitRecord;

class FScatterRecord
{
public:
    FColor3 Attenuation;
    std::shared_ptr<FPDF> PDFPtr;
    bool bSkipPDF;
    FRay SkipPDFRay;
};

class FMaterial
{
public:
    virtual ~FMaterial() = default;

    virtual FColor3 Emit(const FRay& Ray, const FHitRecord& HitRecord, double U, double V, const FPoint3& Point) const;
    virtual bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const;
    virtual double ScatteringPDF(const FRay& Ray, const FHitRecord& HitRecord, const FRay& Scattered) const;
};

class FLambertian : public FMaterial
{
public:
    FLambertian(const FColor3& AlbedoIn);
    FLambertian(std::shared_ptr<FTexture> TextureIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const override;
    double ScatteringPDF(const FRay& Ray, const FHitRecord& HitRecord, const FRay& Scattered) const override;

private:
    std::shared_ptr<FTexture> Texture;
};

class FMetal : public FMaterial
{
public:
    FMetal(const FColor3& AlbedoIn, double FuzzIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const override;

private:
    FColor3 Albedo;
    double Fuzz;
};

class FDielectric : public FMaterial
{
public:
    FDielectric(double RefractionIndexIn);
    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const override;

private:
    double RefractionIndex;

    static double Reflectance(double Cosine, double RefractionIndex);
};

class FDiffuseLight : public FMaterial
{
public:
    FDiffuseLight(const FColor3& EmissionColor);
    FDiffuseLight(std::shared_ptr<FTexture> TextureIn);

    FColor3 Emit(const FRay& Ray, const FHitRecord& HitRecord, double U, double V, const FPoint3& Point) const override;

private:
    std::shared_ptr<FTexture> Texture;
};

class FIsotropic : public FMaterial
{
public:
    FIsotropic(const FColor3& Albedo);
    FIsotropic(std::shared_ptr<FTexture> TextureIn);

    bool Scatter(const FRay& Ray, const FHitRecord& HitRecord, FScatterRecord& ScatterRecord) const override;
    double ScatteringPDF(const FRay& Ray, const FHitRecord& HitRecord, const FRay& Scattered) const override;

private:
    std::shared_ptr<FTexture> Texture;
};

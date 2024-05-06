#pragma once

#include "hittable.h"
#include "material.h"
#include "texture.h"

class FConstantMedia : public FHittable
{
public:
    FConstantMedia(std::shared_ptr<FHittable> BoundaryIn, double DensityIn, std::shared_ptr<FTexture> TextureIn);
    FConstantMedia(std::shared_ptr<FHittable> BoundaryIn, double DensityIn, const FColor3 AlbedoIn);

    bool Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const override;
    FAABB BoundingBox() const override;

private:
    std::shared_ptr<FHittable> Boundary;
    double NegativeInvertedDensity;
    std::shared_ptr<FMaterial> PhaseFunction;
};

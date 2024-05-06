#include "common_defines.h"
#include "rng.h"

#include "constant_media.h"

#include <iostream>

FConstantMedia::FConstantMedia(std::shared_ptr<FHittable> BoundaryIn, double DensityIn, std::shared_ptr<FTexture> TextureIn) : Boundary(BoundaryIn), NegativeInvertedDensity(-1. / DensityIn), PhaseFunction(std::make_shared<FIsotropic>(TextureIn)) {};

FConstantMedia::FConstantMedia(std::shared_ptr<FHittable> BoundaryIn, double DensityIn, const FColor3 AlbedoIn) : Boundary(BoundaryIn), NegativeInvertedDensity(-1. / DensityIn), PhaseFunction(std::make_shared<FIsotropic>(AlbedoIn)) {};

bool FConstantMedia::Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const
{
    const bool bEnableDebug = false;
    const bool bDebugging = bEnableDebug && (RandomDouble() < 0.00001);

    FHitRecord HitRecord1;
    FHitRecord HitRecord2;

    if (!Boundary->Hit(Ray, FInterval::Universe, HitRecord1))
        return false;

    if (!Boundary->Hit(Ray, FInterval(HitRecord1.T + 0.0001, Infinity), HitRecord2))
        return false;

    if (bDebugging)
    {
        std::cout << "\nTMin = " << HitRecord1.T << ", TMax = " << HitRecord2.T << std::endl;
    }

    if (HitRecord1.T < RayInterval.Min)
    {
        HitRecord1.T = RayInterval.Min;
    }

    if (HitRecord2.T > RayInterval.Max)
    {
        HitRecord2.T = RayInterval.Max;
    }

    if (HitRecord1.T >= HitRecord2.T)
    {
        return false;
    }

    if (HitRecord1.T < 0)
    {
        HitRecord1.T = 0;
    }

    auto RayLength = Ray.GetDirection().Length();
    auto DistanceInsideBoundary = (HitRecord2.T - HitRecord1.T) * RayLength;
    auto HitDistance = NegativeInvertedDensity * log(RandomDouble());

    if (HitDistance > DistanceInsideBoundary)
    {
        return false;
    }

    HitRecord.T = HitRecord1.T + HitDistance / RayLength;
    HitRecord.Position = Ray.At(HitRecord.T);

    if (bDebugging)
    {
        std::cout << "Hit distance = " << HitDistance << "\n"
                  << "HitRecord.T = " << HitRecord.T << "\n"
                  << "HitRecord.Position = " << HitRecord.Position.ToString() << std::endl;
    }

    HitRecord.Normal = FVector3(1, 0, 0);
    HitRecord.bFrontFace = true;
    HitRecord.Material = PhaseFunction;

    return true;
}

FAABB FConstantMedia::BoundingBox() const
{
    return Boundary->BoundingBox();
}
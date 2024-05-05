#include "common_defines.h"

#include "hittable.h"

void FHitRecord::SetFaceNormal(const FRay& Ray, const FVector3& OutwardNormal)
{
    bFrontFace = Dot(Ray.GetDirection(), OutwardNormal) < 0;
    Normal = bFrontFace ? OutwardNormal : -OutwardNormal;
}

FTranslate::FTranslate(std::shared_ptr<FHittable> ObjectIn, const FVector3& OffsetIn) : Object(ObjectIn), Offset(OffsetIn)
{
    BBox = Object->BoundingBox() + Offset;
}

bool FTranslate::Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const
{
    FRay OffsetRay(Ray.GetOrigin() - Offset, Ray.GetDirection(), Ray.GetTime());

    if (!Object->Hit(OffsetRay, RayInterval, HitRecord))
    {
        return false;
    }

    HitRecord.Position += Offset;

    return true;
}

FAABB FTranslate::BoundingBox() const
{
    return BBox;
}

FRotateY::FRotateY(std::shared_ptr<FHittable> ObjectIn, double Angle) : Object(ObjectIn)
{
    auto Radians = DegreesToRadians(Angle);
    SinTheta = sin(Radians);
    CosTheta = cos(Radians);

    BBox = Object->BoundingBox();

    FPoint3 Min(Infinity, Infinity, Infinity);
    FPoint3 Max(-Infinity, - Infinity, -Infinity);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                auto X = i * BBox.X.Max + (1 - i) * BBox.X.Min;
                auto Y = j * BBox.Y.Max + (1 - j) * BBox.Y.Min;
                auto Z = k * BBox.Z.Max + (1 - k) * BBox.Z.Min;

                auto NewX = CosTheta * X + SinTheta * Z;
                auto NewZ = -SinTheta * X + CosTheta * Z;

                FVector3 Tester(NewX, Y,NewZ);

                for (int c = 0; c < 3; c++)
                {
                    Min[c] = fmin(Min[c], Tester[c]);
                    Max[c] = fmax(Max[c], Tester[c]);
                }
            }
        }
    }

    BBox = FAABB(Min, Max);
}

bool FRotateY::Hit(const FRay& Ray, FInterval RayInterval, FHitRecord& HitRecord) const
{
    auto Origin = Ray.GetOrigin();
    auto Direction = Ray.GetDirection();

    Origin[0] = CosTheta * Ray.GetOrigin()[0] - SinTheta * Ray.GetOrigin()[2];
    Origin[2] = SinTheta * Ray.GetOrigin()[0] + CosTheta * Ray.GetOrigin()[2];

    Direction[0] = CosTheta * Ray.GetDirection()[0] - SinTheta * Ray.GetDirection()[2];
    Direction[2] = SinTheta * Ray.GetDirection()[0] + CosTheta * Ray.GetDirection()[2];

    FRay RotatedRay(Origin, Direction, Ray.GetTime());

    if (!Object->Hit(RotatedRay, RayInterval, HitRecord))
    {
        return false;
    }

    auto P = HitRecord.Position;
    P[0] =  CosTheta * HitRecord.Position[0] + SinTheta * HitRecord.Position[2];
    P[2] = -SinTheta * HitRecord.Position[0] + CosTheta * HitRecord.Position[2];

    auto Normal = HitRecord.Normal;
    Normal[0] =  CosTheta * HitRecord.Normal[0] + SinTheta * HitRecord.Normal[2];
    Normal[2] = -SinTheta * HitRecord.Normal[0] + CosTheta * HitRecord.Normal[2];

    HitRecord.Position = P;
    HitRecord.Normal = Normal;

    return true;
}

FAABB FRotateY::BoundingBox() const
{
    return BBox;
}
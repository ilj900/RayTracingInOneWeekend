#include "hittable.h"

void FHitRecord::SetFaceNormal(const FRay& Ray, const FVector3& OutwardNormal)
{
    bFrontFace = Dot(Ray.GetDirection(), OutwardNormal) < 0;
    Normal = bFrontFace ? OutwardNormal : -OutwardNormal;
}
#include "quad.h"

FQuad::FQuad(const FPoint3& QIn, const FVector3 UIn, const FVector3 VIn, std::shared_ptr<FMaterial> MaterialIn) : Q(QIn), U(UIn), V(VIn), Material(MaterialIn)
{
    auto N = Cross(U, V);
    Normal = N.GetNormalized();
    D = Dot(Normal, Q);
    W = N / Dot(N, N);

    SetBoundingBox();
};

void FQuad::SetBoundingBox()
{
    auto BBoxDiagonal1 = FAABB(Q, Q + U + V);
    auto BBoxDiagonal2 = FAABB(Q + U, Q + V);

    BBox = FAABB(BBoxDiagonal1, BBoxDiagonal2);
}

FAABB FQuad::BoundingBox() const
{
    return BBox;
}

bool FQuad::Hit(const FRay& Ray, FInterval Interval, FHitRecord& HitRecordOut) const
{
    auto Denominator = Dot(Normal, Ray.GetDirection());

    if (fabs(Denominator) < 1e-8)
    {
        return false;
    }

    auto T = (D - Dot(Normal, Ray.GetOrigin())) / Denominator;

    if (!Interval.Contains(T))
    {
        return false;
    }

    auto IntersectionPoint = Ray.At(T);

    FVector3 PlanarHitPointVector = IntersectionPoint - Q;
    auto Alpha = Dot(W, Cross(PlanarHitPointVector, V));
    auto Beta = Dot(W, Cross(U, PlanarHitPointVector));

    if (!IsInterior(Alpha, Beta, HitRecordOut))
    {
        return false;
    }

    HitRecordOut.T = T;
    HitRecordOut.Position = IntersectionPoint;
    HitRecordOut.Material = Material;
    HitRecordOut.SetFaceNormal(Ray, Normal);

    return true;
}

bool FQuad::IsInterior(double A, double B, FHitRecord& HitRecord) const
{
    static FInterval UnitInterval = FInterval(0, 1);

    if (!UnitInterval.Contains(A) || !UnitInterval.Contains(B))
    {
        return false;
    }

    HitRecord.U = A;
    HitRecord.V = B;

    return true;
}

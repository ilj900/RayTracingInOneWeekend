#include "common_defines.h"
#include "rng.h"

#include "quad.h"

FQuad::FQuad(const FPoint3& QIn, const FVector3 UIn, const FVector3 VIn, std::shared_ptr<FMaterial> MaterialIn) : Q(QIn), U(UIn), V(VIn), Material(MaterialIn)
{
    auto N = Cross(U, V);
    Normal = N.GetNormalized();
    D = Dot(Normal, Q);
    W = N / Dot(N, N);

    Area = N.Length2();

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

double FQuad::PDFValue(const FPoint3& Origin, const FVector3& Direction) const
{
    FHitRecord HitRecord;
    if (!this->Hit(FRay(Origin, Direction), FInterval(0.001, Infinity), HitRecord))
    {
        return 0;
    }

    auto Distance2 = HitRecord.T * HitRecord.T * Direction.Length2();
    auto Cosine = fabs(Dot(Direction, HitRecord.Normal) / Direction.Length());

    return Distance2 / (Cosine * Area);
}

FVector3 FQuad::Random(const FPoint3& Origin) const
{
    auto P = Q + (RandomDouble() * U) + (RandomDouble() * V);
    return P - Origin;
}

std::shared_ptr<FHittableList> Box(const FPoint3& A, const FPoint3& B, std::shared_ptr<FMaterial> Material)
{
    auto Sides = std::make_shared<FHittableList>();

    auto Min = FPoint3(fmin(A.X, B.X), fmin(A.Y, B.Y), fmin(A.Z, B.Z));
    auto Max = FPoint3(fmax(A.X, B.X), fmax(A.Y, B.Y), fmax(A.Z, B.Z));

    auto DX = FVector3(Max.X - Min.X, 0, 0);
    auto DY = FVector3(0, Max.Y - Min.Y, 0);
    auto DZ = FVector3(0, 0, Max.Z - Min.Z);

    Sides->Add(std::make_shared<FQuad>(FPoint3(Min.X, Min.Y, Max.Z),  DX,  DY, Material));
    Sides->Add(std::make_shared<FQuad>(FPoint3(Max.X, Min.Y, Max.Z), -DZ,  DY, Material));
    Sides->Add(std::make_shared<FQuad>(FPoint3(Max.X, Min.Y, Min.Z), -DX,  DY, Material));
    Sides->Add(std::make_shared<FQuad>(FPoint3(Min.X, Min.Y, Min.Z),  DZ,  DY, Material));
    Sides->Add(std::make_shared<FQuad>(FPoint3(Min.X, Max.Y, Max.Z),  DX, -DZ, Material));
    Sides->Add(std::make_shared<FQuad>(FPoint3(Min.X, Min.Y, Min.Z),  DX,  DZ, Material));

    return Sides;
}

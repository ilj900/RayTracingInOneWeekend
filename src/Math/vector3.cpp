#include "vector3.h"

#include <cmath>
#include <stdexcept>

///****************************************************************
///FVector3 and it's operations
///****************************************************************

FVector3 FVector3::GetNormalized() const
{
    float L = std::sqrt(X * X + Y * Y + Z * Z);

    if (L <= 0.000001f)
    {
        throw std::runtime_error("Failed to normalize FVector3");
    }

    return {X / L, Y / L, Z / L};
}

FVector3& FVector3::Normalize()
{
    auto L = Length();
    X /= L;
    Y /= L;
    Z /= L;

    return *this;
}

float FVector3::Length() const
{
    return sqrt(X * X + Y * Y + Z * Z);
}

float FVector3::Length2() const
{
    return X * X + Y * Y + Z * Z;
}

float Dot(const FVector3& A, const FVector3& B)
{
    return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

FVector3 Cross(const FVector3& A, const FVector3& B)
{
    return {A.Y * B.Z - A.Z * B.Y,
            A.Z * B.X - A.X * B.Z,
            A.X * B.Y - A.Y * B.X};
}

FVector3 Reflect(const FVector3& Vector, const FVector3& Normal)
{
    return Vector - 2.f * Dot(Vector, Normal) * Normal;
}


FVector3 Refract(const FVector3& Ray, const FVector3& Normal, float EtaiToEtat)
{
    auto CosTheta = fmin(Dot(-Ray, Normal), 1);
    FVector3 RayOutPerp = EtaiToEtat * (Ray + CosTheta * Normal);
    FVector3 RayOutParallel = -sqrt(fabs(1.f - RayOutPerp.Length2())) * Normal;
    return RayOutPerp + RayOutParallel;
}

std::string FVector3::ToString() const
{
    std::string Result = "FVector3(";
    Result += std::to_string(X) + ", ";
    Result += std::to_string(Y) + ", ";
    Result += std::to_string(Z) + ")";

    return Result;
}

bool FVector3::NearZero() const
{
    static float Epsilon = 1e-8;
    return (std::fabs(X) < Epsilon && std::fabs(Y) < Epsilon && std::fabs(Z) < Epsilon);
}

FVector3 operator-(const FVector3& A)
{
    return {-A.X, -A.Y, -A.Z};
}

bool operator==(const FVector3& A, const FVector3& B)
{
    return (A.X == B.X && A.Y == B.Y && A.Z == B.Z);
}

FVector3& operator+=(FVector3& A, const FVector3& B)
{
    A.X += B.X;
    A.Y += B.Y;
    A.Z += B.Z;

    return A;
}

FVector3& operator-=(FVector3& A, const FVector3& B)
{
    A.X -= B.X;
    A.Y -= B.Y;
    A.Z -= B.Z;

    return A;
}

FVector3& operator*=(FVector3& A, const FVector3& B)
{
    A.X *= B.X;
    A.Y *= B.Y;
    A.Z *= B.Z;

    return A;
}

FVector3& operator*=(FVector3& A, float Val)
{
    A.X *= Val;
    A.Y *= Val;
    A.Z *= Val;

    return A;
}

FVector3& operator/=(FVector3& A, float Val)
{
    A.X /= Val;
    A.Y /= Val;
    A.Z /= Val;

    return A;
}

FVector3 operator+(const FVector3& A, const FVector3& B)
{
    return {A.X + B.X, A.Y + B.Y, A.Z + B.Z};
}

FVector3 operator-(const FVector3& A, const FVector3& B)
{
    return {A.X - B.X, A.Y - B.Y, A.Z - B.Z};
}

FVector3 operator*(const FVector3& A, const FVector3& B)
{
    return {A.X * B.X, A.Y * B.Y, A.Z * B.Z};
}

FVector3 operator*(const FVector3& A, float Val)
{
    return {A.X * Val, A.Y * Val, A.Z * Val};
}

FVector3 operator*(float Val, const FVector3& A)
{
    return A * Val;
}

FVector3 operator/(const FVector3& A, float Val)
{
    return {A.X / Val, A.Y / Val, A.Z / Val};
}

FVector3 operator/(float Val, const FVector3& A)
{
    return A / Val;
}

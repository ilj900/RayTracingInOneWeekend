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

std::string FVector3::ToString() const
{
    std::string Result = "FVector3(";
    Result += std::to_string(X) + ", ";
    Result += std::to_string(Y) + ", ";
    Result += std::to_string(Z) + ")";

    return Result;
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
    return {A.Y * B.X, A.Y * B.Y, A.Z * B.Z};
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

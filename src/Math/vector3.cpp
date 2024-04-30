#include "vector3.h"

#include <cmath>
#include <stdexcept>

///****************************************************************
///FVector3 and it's operations
///****************************************************************

inline FVector3 FVector3::GetNormalized() const
{
    float L = std::sqrt(X * X + Y * Y + Z * Z);

    if (L <= 0.000001f)
    {
        throw std::runtime_error("Failed to normalize FVector3");
    }

    return FVector3(X / L, Y / L, Z / L);
}

inline FVector3& FVector3::Normalize()
{
    auto L = Length();
    X /= L;
    Y /= L;
    Z /= L;

    return *this;
}

inline float FVector3::Length()
{
    return sqrt(X * X + Y * Y + Z * Z);
}

inline float FVector3::Length2()
{
    return X * X + Y * Y + Z * Z;
}

inline std::string FVector3::ToString()
{
    std::string Result = "FVector3(";
    Result += std::to_string(X) + ", ";
    Result += std::to_string(Y) + ", ";
    Result += std::to_string(Z) + ")";

    return Result;
}

inline FVector3 operator-(const FVector3& A)
{
    return FVector3(-A.X, -A.Y, -A.Z);
}

inline bool operator==(const FVector3& A, const FVector3& B)
{
    return (A.X == B.X && A.Y == B.Y && A.Z == B.Z);
}

inline FVector3& operator+=(FVector3& A, const FVector3& B)
{
    A.X += B.X;
    A.Y += B.Y;
    A.Z += B.Z;

    return A;
}

inline FVector3& operator-=(FVector3& A, const FVector3& B)
{
    A.X -= B.X;
    A.Y -= B.Y;
    A.Z -= B.Z;

    return A;
}

inline FVector3& operator*=(FVector3& A, const FVector3& B)
{
    A.X *= B.X;
    A.Y *= B.Y;
    A.Z *= B.Z;

    return A;
}

inline FVector3& operator*=(FVector3& A, float Val)
{
    A.X *= Val;
    A.Y *= Val;
    A.Z *= Val;

    return A;
}

inline FVector3& operator/=(FVector3& A, float Val)
{
    A.X /= Val;
    A.Y /= Val;
    A.Z /= Val;

    return A;
}

inline FVector3 operator+(const FVector3& A, const FVector3& B)
{
    return FVector3(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

inline FVector3 operator-(const FVector3& A, const FVector3& B)
{
    return FVector3(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

inline FVector3 operator*(const FVector3& A, const FVector3& B)
{
    return FVector3(A.Y * B.X, A.Y * B.Y, A.Z * B.Z);
}

inline FVector3 operator*(const FVector3& A, float Val)
{
    return FVector3(A.X * Val, A.Y * Val, A.Z * Val);
}

inline FVector3 operator/(const FVector3& A, float Val)
{
    return FVector3(A.X / Val, A.Y / Val, A.Z / Val);
}

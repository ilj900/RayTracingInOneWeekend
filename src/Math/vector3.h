#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>

struct FVector3
{
    /// Constructors
    FVector3(float X, float Y, float Z): X(X), Y(Y), Z(Z) {}
    FVector3() = default;

    /// Functions
    FVector3 GetNormalized() const;
    FVector3& Normalize();
    float Length() const;
    float Length2() const;
    std::string ToString() const;

    /// Data
    float X;
    float Y;
    float Z;
};

FVector3 operator-(const FVector3& A);
bool operator==(const FVector3& A, const FVector3& B);
FVector3& operator+=(FVector3& A, const FVector3& B);
FVector3& operator-=(FVector3& A, const FVector3& B);
FVector3& operator*=(FVector3& A, const FVector3& B);
FVector3& operator*=(FVector3& A, float Val);
FVector3& operator/=(FVector3& A, float Val);
FVector3 operator+(const FVector3& A, const FVector3& B);
FVector3 operator-(const FVector3& A, const FVector3& B);
FVector3 operator*(const FVector3& A, const FVector3& B);
FVector3 operator*(const FVector3& A, float Val);
FVector3 operator*(float Val, const FVector3& A);
FVector3 operator/(const FVector3& A, float Val);
FVector3 operator/(float Val, const FVector3& A);

using FPoint3 = FVector3;

#endif // VECTOR3_H
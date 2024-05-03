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
    bool NearZero() const;

    /// Data
    float X = 0;
    float Y = 0;
    float Z = 0;
};

float Dot(const FVector3& A, const FVector3& B);
FVector3 Reflect(const FVector3& Vector, const FVector3& Normal);
FVector3 Refract(const FVector3& Ray, const FVector3& Normal, float EtaiToEtat);

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
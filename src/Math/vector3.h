#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>

struct FVector3
{
    /// Constructors
    FVector3(double X, double Y, double Z): X(X), Y(Y), Z(Z) {}
    FVector3() = default;

    /// Functions
    FVector3 GetNormalized() const;
    FVector3& Normalize();
    double Length() const;
    double Length2() const;
    std::string ToString() const;
    bool NearZero() const;

    /// Data
    double X = 0;
    double Y = 0;
    double Z = 0;
};

double Dot(const FVector3& A, const FVector3& B);
FVector3 Cross(const FVector3& A, const FVector3& B);
FVector3 Reflect(const FVector3& Vector, const FVector3& Normal);
FVector3 Refract(const FVector3& Ray, const FVector3& Normal, double EtaiToEtat);

FVector3 operator-(const FVector3& A);
bool operator==(const FVector3& A, const FVector3& B);
FVector3& operator+=(FVector3& A, const FVector3& B);
FVector3& operator-=(FVector3& A, const FVector3& B);
FVector3& operator*=(FVector3& A, const FVector3& B);
FVector3& operator*=(FVector3& A, double Val);
FVector3& operator/=(FVector3& A, double Val);
FVector3 operator+(const FVector3& A, const FVector3& B);
FVector3 operator-(const FVector3& A, const FVector3& B);
FVector3 operator*(const FVector3& A, const FVector3& B);
FVector3 operator*(const FVector3& A, double Val);
FVector3 operator*(double Val, const FVector3& A);
FVector3 operator/(const FVector3& A, double Val);
FVector3 operator/(double Val, const FVector3& A);

using FPoint3 = FVector3;

#endif // VECTOR3_H
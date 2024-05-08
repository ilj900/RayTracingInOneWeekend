#pragma once

#include "vector3.h"

class FONB
{
public:
    FONB() = default;

    FVector3 Local(double A, double B, double C)const;
    FVector3 Local(const FVector3& A) const;
    void BuildFromW(const FVector3& In);

    FVector3 operator[](int i) const;
    FVector3& operator[](int i);

private:
    FVector3 U;
    FVector3 V;
    FVector3 W;
};
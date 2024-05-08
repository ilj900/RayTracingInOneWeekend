#pragma once

#include "onb.h"

class FPDF
{
public:
    virtual ~FPDF() = default;

    virtual double Value(const FVector3 Direction) const = 0;
    virtual FVector3 Generate() const = 0;
};

class FSpherePDF : public FPDF
{
public:
    FSpherePDF() = default;

    double Value(const FVector3 Direction) const override;
    FVector3 Generate() const override;
};

class FCosinePDF : public FPDF
{
public:
    FCosinePDF(const FVector3& W);

    double Value(const FVector3 Direction) const override;
    FVector3 Generate() const override;

private:
    FONB UVW;
};
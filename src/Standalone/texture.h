#pragma once

#include "color3.h"
#include "image_wrapper.h"

#include <memory>

class FTexture
{
public:
    virtual ~FTexture() = default;

    virtual FColor3 Value(double U, double  V, const FPoint3& Position) const = 0;
};

class FSolidColor : public FTexture
{
public:
    FSolidColor(const FColor3& AlbedoIn);
    FSolidColor(double R, double G, double B);

    FColor3 Value(double U, double  V, const FPoint3& Position) const override;

private:
    FColor3 Albedo;
};

class FCheckerTexture : public FTexture
{
public:
    FCheckerTexture(double Scale, std::shared_ptr<FTexture> EvenIn, std::shared_ptr<FTexture> OddIn);
    FCheckerTexture(double Scale, const FColor3& ColorEven, const FColor3& ColorOdd);

    FColor3 Value(double U, double  V, const FPoint3& Position) const override;

private:
    double IncScale;
    std::shared_ptr<FTexture> Even;
    std::shared_ptr<FTexture> Odd;
};

class FImageTexture : public FTexture
{
public:
    FImageTexture(const std::string& Filename);

    FColor3 Value(double U, double  V, const FPoint3& Position) const override;

private:
    FImageWrapper Image;
};

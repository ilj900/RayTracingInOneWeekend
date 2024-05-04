#include "interval.h"

#include "texture.h"

#include <cmath>

FSolidColor::FSolidColor(const FColor3 &AlbedoIn) : Albedo(AlbedoIn) {};

FSolidColor::FSolidColor(double R, double G, double B) : FSolidColor(FColor3(R, G, B)) {};

FColor3 FSolidColor::Value(double U, double V, const FPoint3 &Position) const
{
    return Albedo;
};

FCheckerTexture::FCheckerTexture(double Scale, std::shared_ptr<FTexture> EvenIn, std::shared_ptr<FTexture> OddIn) : IncScale(1. / Scale), Even(EvenIn), Odd(OddIn) {};

FCheckerTexture::FCheckerTexture(double Scale, const FColor3& ColorEven, const FColor3& ColorOdd) : IncScale(1. / Scale), Even(std::make_shared<FSolidColor>(ColorEven)), Odd(std::make_shared<FSolidColor>(ColorOdd)) {};

FColor3 FCheckerTexture::Value(double U, double  V, const FPoint3& Position) const
{
    auto XInteger = int(std::floor(IncScale * Position.X));
    auto YInteger = int(std::floor(IncScale * Position.Y));
    auto ZInteger = int(std::floor(IncScale * Position.Z));

    bool bIsEven = (XInteger + YInteger + ZInteger) % 2 == 0;

    return bIsEven ? Even->Value(U, V, Position) : Odd->Value(U, V, Position);
};

FImageTexture::FImageTexture(const std::string& Filename) : Image(Filename) {};

FColor3 FImageTexture::Value(double U, double  V, const FPoint3& Position) const
{
    if (Image.Height() <= 0)
    {
        return FColor3(0, 1, 1);
    }

    U = FInterval(0, 1).Clamp(U);
    V = 1. - FInterval(0, 1).Clamp(V);

    auto i = int(U * Image.Width());
    auto j = int(V * Image.Height());
    auto Pixel = Image.PixelData(i, j);

    auto ColorScale = 1. / 255.;
    return FColor3(ColorScale * Pixel[0], ColorScale * Pixel[1], ColorScale * Pixel[2]);
};

FColor3 FNoiseTexture::Value(double U, double  V, const FPoint3& Position) const
{
    return FColor3(1, 1, 1) * Noise.Noise(Position);
}

#include "color3.h"

void WriteColor(std::vector<float>& Data, const FColor3& PixelColor)
{
    Data.push_back(PixelColor.X);
    Data.push_back(PixelColor.Y);
    Data.push_back(PixelColor.Z);
}

void WriteColor(std::vector<unsigned char>& Data, const FColor3& PixelColor)
{
    Data.push_back(255.999f * PixelColor.X);
    Data.push_back(255.999f * PixelColor.Y);
    Data.push_back(255.999f * PixelColor.Z);
}

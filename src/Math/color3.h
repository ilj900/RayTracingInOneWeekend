#ifndef COLOR3_H
#define COLOR3_H

#include "vector3.h"

#include <vector>

using FColor3 = FVector3;

void WriteColor(std::vector<float>& Data, const FColor3& PixelColor)
{
    Data.push_back(PixelColor.X);
    Data.push_back(PixelColor.Y);
    Data.push_back(PixelColor.Z);
}

void WriteColor(std::vector<unsigned char>& Data, const FColor3& PixelColor)
{
    Data.push_back(255.999 * PixelColor.X);
    Data.push_back(255.999 * PixelColor.Y);
    Data.push_back(255.999 * PixelColor.Z);
}

#endif // COLOR3_H
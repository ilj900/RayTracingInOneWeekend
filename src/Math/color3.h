#ifndef COLOR3_H
#define COLOR3_H

#include "vector3.h"

#include <vector>

using FColor3 = FVector3;

void WriteColor(std::vector<float>& Data, const FColor3& PixelColor);

void WriteColor(std::vector<unsigned char>& Data, const FColor3& PixelColor);

#endif // COLOR3_H
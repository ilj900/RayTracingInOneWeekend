#pragma once

#include "color3.h"

#include <vector>

class FImageWrapper
{
public:
    FImageWrapper() = default;
    FImageWrapper(const std::string& ImageFilename);

    ~FImageWrapper();

    bool Load(const std::string& ImageFilename);
    uint32_t Width() const;
    uint32_t Height()const;

    const unsigned char* PixelData(int X, int Y) const;

private:
    static int Clamp(int X, int Low, int High);
    static unsigned char FloatToByte(float Value);
    void ConvertToBytes();

    const int BytesPerPixel = 3;
    float* ImageDataFloat = nullptr;
    unsigned char* ImageDataByte = nullptr;
    int ImageWidth = 0;
    int ImageHeight = 0;
    int BytesPerScanline = 0;

};

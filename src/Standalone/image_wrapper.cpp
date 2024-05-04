
#define STBI_FREE
#include "stb_image.h"

#include "image_wrapper.h"

#include <iostream>

FImageWrapper::FImageWrapper(const std::string& ImageFilename)
{
    if (Load(ImageFilename)) return;
    if (Load("images/" + ImageFilename)) return;
    if (Load("../images/" + ImageFilename)) return;
    if (Load("../../images/" + ImageFilename)) return;
    if (Load("../../../images/" + ImageFilename)) return;
    if (Load("../../../../images/" + ImageFilename)) return;

    std::cerr << "ERROR: Could not load image file: " << ImageFilename << std::endl;
}

FImageWrapper::~FImageWrapper()
{
    delete[] ImageDataByte;
    STBI_FREE(ImageDataFloat);
}

bool FImageWrapper::Load(const std::string& ImageFilename)
{
    auto N = BytesPerPixel;
    ImageDataFloat = stbi_loadf(ImageFilename.c_str(), &ImageWidth, &ImageHeight, &N, BytesPerPixel);
    if (ImageDataFloat == nullptr)
    {
        return false;
    }

    BytesPerScanline = ImageWidth * BytesPerPixel;
    ConvertToBytes();
    return true;

}

uint32_t FImageWrapper::Width() const
{
    return (ImageDataFloat == nullptr ? 0 : ImageWidth);
}

uint32_t FImageWrapper::Height() const
{
    return (ImageDataFloat == nullptr ? 0 : ImageHeight);
}


const unsigned char* FImageWrapper::PixelData(int X, int Y) const
{
    static unsigned char Magenta[] = {255, 0, 255};
    if (ImageDataByte == nullptr)
    {
        return Magenta;
    }

    X =Clamp(X, 0, ImageWidth);
    Y =Clamp(Y, 0, ImageHeight);

    return ImageDataByte + Y * BytesPerScanline + X * BytesPerPixel;
}


int FImageWrapper::Clamp(int X, int Low, int High)
{
    if (X < Low)
    {
        return Low;
    }

    if (X < High)
    {
        return X;
    }

    return High - 1;
}

unsigned char FImageWrapper::FloatToByte(float Value)
{
    if (Value <= 0.)
    {
        return 0.;
    }

    if (1. <= Value)
    {
        return 255;
    }

    return static_cast<unsigned char>(256. * Value);
}

void FImageWrapper::ConvertToBytes()
{
    int TotalBytes = ImageWidth * ImageHeight * BytesPerPixel;

    ImageDataByte = new unsigned char[TotalBytes];

    auto* BPTR = ImageDataByte;
    auto* FPTR = ImageDataFloat;

    for (auto i = 0; i < TotalBytes; i++, FPTR++, BPTR++)
    {
        *BPTR = FloatToByte(*FPTR);
    }
}


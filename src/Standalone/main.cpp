#include "stb_image_write.h"
#include "tinyexr.h"

#include "color3.h"

#include <iostream>

int main()
{
    uint32_t ImageWidth = 1920;
    uint32_t ImageHeight = 1080;

    std::vector<float> FloatImageData;
    FloatImageData.reserve(ImageWidth * ImageHeight * 3);
    std::vector<unsigned char> UnsignedCharImageData;
    UnsignedCharImageData.reserve(ImageWidth * ImageHeight * 3);

    for (uint32_t i = 0; i < ImageHeight; ++i)
    {
        std::cout << "Scanline remaining: " << (ImageHeight - i) << std::endl;

        for(uint32_t j = 0; j < ImageWidth; ++j)
        {
            FColor3 PixelColor = {float(j) / float(ImageWidth), float(i) / float(ImageHeight), 0};
            WriteColor(FloatImageData, PixelColor);
            WriteColor(UnsignedCharImageData, PixelColor);
        }
    }

    std::cout << "Rendering finished." << std::endl;

    const char* Err = NULL;
    SaveEXR(FloatImageData.data(), ImageWidth, ImageHeight, 3, false, "Result.exr", &Err);

    stbi_write_bmp("Result.bmp", ImageWidth, ImageHeight, 3, UnsignedCharImageData.data());
}
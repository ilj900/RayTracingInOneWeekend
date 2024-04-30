#include "stb_image_write.h"
#include "tinyexr.h"

#include <vector>

int main()
{
    uint32_t ImageWidth = 1920;
    uint32_t ImageHeight = 1080;

    std::vector<float> FloatImageData(ImageWidth * ImageHeight * 3);
    std::vector<unsigned char> UnsignedCharImageData(ImageWidth * ImageHeight * 3);

    for (uint32_t i = 0; i < ImageHeight; ++i)
    {
        for(uint32_t j = 0; j < ImageWidth; ++j)
        {
            FloatImageData[(i * ImageWidth + j) * 3] = float(j) / float(ImageWidth);
            FloatImageData[(i * ImageWidth + j) * 3 + 1] = float(i) / float(ImageHeight);;
            FloatImageData[(i * ImageWidth + j) * 3 + 2] = 0;

            UnsignedCharImageData[(i * ImageWidth + j) * 3] = 255.999 * FloatImageData[(i * ImageWidth + j) * 3];
            UnsignedCharImageData[(i * ImageWidth + j) * 3 + 1] = 255.999 * FloatImageData[(i * ImageWidth + j) * 3 + 1];
            UnsignedCharImageData[(i * ImageWidth + j) * 3 + 2] = 255.999 * FloatImageData[(i * ImageWidth + j) * 3 + 2];
        }
    }

    const char* Err = NULL;
    SaveEXR(FloatImageData.data(), ImageWidth, ImageHeight, 3, false, "Result.exr", &Err);

    stbi_write_bmp("Result.bmp", ImageWidth, ImageHeight, 3, UnsignedCharImageData.data());
}
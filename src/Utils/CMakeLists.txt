set(INCLUDE_TINY_EXR
        tinyexr.h
        miniz.h)

set(SOURCE_TINY_EXR
        tinyexr.cpp
        miniz.c)

add_library(TinyExr STATIC ${SOURCE_TINY_EXR} ${INCLUDE_TINY_EXR})
target_include_directories(TinyExr PRIVATE .)

set(INCLUDE_STB_IMAGE
        stb_image.h
        stb_image_write.h)

set(SOURCE_STB_IMAGE
        stb_image.cpp
        stb_image_write.cpp)

add_library(StbImage STATIC ${SOURCE_STB_IMAGE} ${INCLUDE_STB_IMAGE})

set (INCLUDE_UTILS
        rng.h)

set (SOURCE_UTILS
        rng.cpp)

add_library(UTILS STATIC ${SOURCE_UTILS} ${INCLUDE_UTILS})

target_include_directories(Standalone PUBLIC .)
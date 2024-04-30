set(INCLUDE_MATH
        color3.h
        vector3.h)

set(SOURCE_MATH
        vector3.cpp)

add_library(Math STATIC ${SOURCE_MATH} ${INCLUDE_MATH})
target_include_directories(TinyExr PRIVATE .)

target_include_directories(Standalone PUBLIC .)
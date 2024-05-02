set(INCLUDE
        common_defines.h
        hittable.h
        hittable_list.h
        sphere.h)

set(SOURCE
        hittable.cpp
        hittable_list.cpp
        main.cpp
        sphere.cpp)

add_executable(Standalone ${SOURCE} ${INCLUDE})

target_link_libraries(Standalone TinyExr StbImage Math)
set(INCLUDE
        camera.h
        common_defines.h
        hittable.h
        hittable_list.h
        interval.h
        material.h
        sphere.h)

set(SOURCE
        camera.cpp
        hittable.cpp
        hittable_list.cpp
        interval.cpp
        main.cpp
        material.cpp
        sphere.cpp)

add_executable(Standalone ${SOURCE} ${INCLUDE})

target_link_libraries(Standalone TinyExr StbImage Math Utils)
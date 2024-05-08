set(INCLUDE
        aabb.h
        bvh_node.h
        camera.h
        common_defines.h
        constant_media.h
        hittable.h
        hittable_list.h
        interval.h
        image_wrapper.h
        material.h
        onb.h
        pdf.h
        perlin.h
        quad.h
        sphere.h
        texture.h)

set(SOURCE
        aabb.cpp
        bvh_node.cpp
        camera.cpp
        constant_media.cpp
        hittable.cpp
        hittable_list.cpp
        interval.cpp
        image_wrapper.cpp
        main.cpp
        material.cpp
        onb.cpp
        pdf.cpp
        perlin.cpp
        quad.cpp
        sphere.cpp
        texture.cpp)

add_executable(Standalone ${SOURCE} ${INCLUDE})

target_link_libraries(Standalone TinyExr StbImage Math Utils)
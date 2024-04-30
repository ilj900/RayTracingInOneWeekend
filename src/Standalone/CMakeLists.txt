set(SOURCE
        main.cpp)

add_executable(Standalone ${SOURCE})

target_link_libraries(Standalone TinyExr StbImage)
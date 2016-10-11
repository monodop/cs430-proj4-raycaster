cmake_minimum_required(VERSION 3.6)
project(cs430_proj3_illumination)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/c/main.c src/c/ppm_header.c src/headers/ppm_header.h src/c/helpers.c src/headers/helpers.h src/c/ppm.c src/c/ppm3.c src/headers/ppm3.h src/c/ppm6.c src/headers/ppm6.h src/headers/ppm.h src/headers/image.h src/c/json.c src/headers/json.h src/headers/scene.h src/headers/vec.h src/c/scene.c src/c/image.c src/headers/raycast.h src/c/raycast.c src/headers/interpolate.h src/c/interpolate.c)
add_executable(cs430_proj3_illumination ${SOURCE_FILES})
target_link_libraries(cs430_proj3_illumination pthread)
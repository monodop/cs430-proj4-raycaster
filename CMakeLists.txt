cmake_minimum_required(VERSION 3.6)
project(cs430_proj1_images)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/c/main.c src/c/ppm_header.c src/headers/ppm_header.h src/c/helpers.c src/headers/helpers.h src/c/ppm.c src/c/ppm3.c src/headers/ppm3.h src/c/ppm6.c src/headers/ppm6.h)
add_executable(cs430_proj1_images ${SOURCE_FILES})
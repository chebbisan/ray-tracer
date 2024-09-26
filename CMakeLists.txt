cmake_minimum_required(VERSION 3.28)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    RayTracer
    main.cpp
    src/vec3.h
    src/color.h
    src/ray.h
    src/hittable.h
    src/sphere.h
    src/hittable_list.h
    src/util.h
    src/interval.h
    src/camera.h
    src/material.h
)

add_compile_options(-fsanitize=thread)

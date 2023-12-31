cmake_minimum_required(VERSION 3.27)
project(New2024_Year C)

set(CMAKE_C_STANDARD 11)

add_executable(New2024_Year
        main.c
        loging/log.c
        prj_types/Array_type.c
        prj_types/String_array_struct.c
        prj_types/String_struct.c
        prj_types/String_struct_formatting.c
        colors.c
        random/statistical_random.c
        bins.c
        snow.h)

target_link_libraries(New2024_Year m)
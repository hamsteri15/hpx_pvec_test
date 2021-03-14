
cmake_minimum_required(VERSION 2.8)

project(minimal)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)




find_package(HPX REQUIRED)



add_hpx_executable(minimal
    SOURCES minimal.cpp
    COMPONENT_DEPENDENCIES iostreams partitioned_vector)
    
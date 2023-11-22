cmake_minimum_required(VERSION 3.23)

project(sycl-fluid)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fsycl -std=c++17")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lOpenCL -lsycl")

add_subdirectory(deps)
add_subdirectory(src)
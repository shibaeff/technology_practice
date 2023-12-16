cmake_minimum_required(VERSION 3.26)
project(genetic)

set(CMAKE_CXX_STANDARD 17)
add_executable(a game.h algo.h main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(a PUBLIC OpenMP::OpenMP_CXX)
endif()

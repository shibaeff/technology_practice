cmake_minimum_required(VERSION 3.26)
project(bin)

set(CMAKE_CXX_STANDARD 14)

include_directories(code)

add_executable(bin
        main.cpp
        code/Annealing.hpp
        code/Schedule.hpp
        code/Temperature.cpp
        code/Temperature.hpp
        code/Format.hpp
        code/Mutation.hpp

)

cmake_minimum_required(VERSION 3.26)
project(genetic)

set(CMAKE_CXX_STANDARD 17)
add_executable(a game.h algo.h main.cpp)
add_executable(visual game.h algo.h  visual.cpp)
find_package(Curses REQUIRED)


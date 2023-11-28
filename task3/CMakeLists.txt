cmake_minimum_required(VERSION 3.16)
project(functions)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(Function src/Factory.hpp src/Types.hpp src/Types.cpp
        src/algorithm.hpp)

add_library(funclib ${Function}
        src/algorithm.hpp)

add_executable(test src/tests/semantic_test.cpp src/tests/calc_test.cpp)
target_link_libraries(test funclib)
target_link_libraries(test gtest_main)
add_test(NAME test COMMAND test)

add_executable(main main.cpp)
target_link_libraries(main funclib)
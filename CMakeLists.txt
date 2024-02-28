cmake_minimum_required(VERSION 3.08)
project(MyMatrix2)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(google_tests)
add_executable(MyMatrix2 MyMatrix.hpp main.cpp)
add_executable(TestMyMatrix2 MyMatrix.hpp test.cpp)

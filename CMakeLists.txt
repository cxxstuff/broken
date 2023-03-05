cmake_minimum_required(VERSION 3.8)
project(BrokenTest CXX)

add_definitions(-DBROKEN_EXAMPLE_TEST)
add_executable(broken_test
  broken.cpp
  broken.h
  broken_main.cpp
  broken_example.cpp
)

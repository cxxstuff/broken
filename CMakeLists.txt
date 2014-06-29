CMake_Minimum_Required(VERSION 2.8.8)
Project(BrokenTest C CXX)

Add_Definitions(-DBROKEN_EXAMPLE_TEST)
Add_Executable(broken_test 
  broken.cpp
  broken.h
  broken_main.cpp
  broken_example.cpp
)

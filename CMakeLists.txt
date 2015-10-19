#
# Runway CMakeLists
# (c) 2015 Joshua Brunhuber
#

cmake_minimum_required(VERSION 2.8.8)
project(Runway)
set(CMAKE_CXX_FLAGS "-std=gnu++0x -fno-rtti")
add_subdirectory(src)
add_subdirectory(test)
enable_testing()
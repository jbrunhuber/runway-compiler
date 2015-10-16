#
# Runway CMakeLists
# (c) 2015 Joshua Brunhuber
#

cmake_minimum_required(VERSION 2.8.8)
project(Runway)
set(CMAKE_CXX_FLAGS "-std=gnu++0x -fno-rtti")
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk) #xcode 7 fix
add_subdirectory(src)
add_subdirectory(test)
enable_testing()
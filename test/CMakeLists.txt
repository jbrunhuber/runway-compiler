#
# Runway CMakeLists
# configures test sources
# (c) 2015 Joshua Brunhuber
#

# We need llvm, gtets and thread libraries
find_package(LLVM REQUIRED CONFIG)
find_package(GTest REQUIRED)
find_package (Threads)

add_definitions(${LLVM_DEFINITIONS})

# include directories from llvm and gtest
include_directories(${GTEST_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS} "../include")

#define our test targets
add_executable(test_target runway_test.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link the libraries
target_link_libraries(test_target runway_library ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${llvm_libs})

# Add the tests
add_test(Tests test_target)

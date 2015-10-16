#
# Runway CMakeLists
# configures runway sources
# (c) 2015 Joshua Brunhuber
#

# We need llvm libraries
find_package(LLVM REQUIRED CONFIG)

# notify about the found llvm libraries
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_definitions(${LLVM_DEFINITIONS})

# We need llvm include directories
include_directories(${LLVM_INCLUDE_DIRS} "../include")

# We build a library with all our soures (except of the main.cpp). We'll link the library against the main.cpp later
FILE(GLOB_RECURSE runway_src "runway/*.cpp")

# Now we add the runway libary and our executable
add_library(runway_library ${runway_src})
add_executable(runway_target main.cpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM and runway libraries
target_link_libraries(runway_target runway_library ${llvm_libs})
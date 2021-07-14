cmake_minimum_required(VERSION 3.15)

include("CMake/Toolchain/GCC_ARM.cmake")

project(CortexNetworkTestProject ASM C CXX)
 
add_subdirectory(CortexNetworkTestApp)

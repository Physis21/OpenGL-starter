cmake_minimum_required(VERSION 3.10.0)
project(OpenGL-starter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor)
add_subdirectory(src)


include(CTest)
enable_testing()

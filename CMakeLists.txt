cmake_minimum_required(VERSION 3.22.0)
project(OpenGL-starter VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
# Disable GNU extensions
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor)
add_subdirectory(lib)
add_subdirectory(src)


include(CTest)
enable_testing()

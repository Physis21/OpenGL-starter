cmake_minimum_required(VERSION 3.22.0)
project(OpenGL-starter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(vendor)
FetchContent_MakeAvailable(glm) # Now glm::glm is available
add_subdirectory(lib)
add_subdirectory(src)


include(CTest)
enable_testing()

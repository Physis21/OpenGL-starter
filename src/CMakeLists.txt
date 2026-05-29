set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# To use the new OpenGL, and not compatibility version
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

add_executable(OpenGL-starter main.cpp)
target_link_libraries(OpenGL-starter
    PRIVATE
        glfw
        glad
        OpenGL::GL
)

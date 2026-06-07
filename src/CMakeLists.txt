set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(OpenGL-starter main.cpp)
target_sources(OpenGL-starter
    PRIVATE main.cpp
    PUBLIC FILE_SET HEADERS FILES main.hpp
)
target_link_libraries(OpenGL-starter
    PRIVATE
        glad
        glfw
        glm::glm
        imgui
        RenderEngine
)

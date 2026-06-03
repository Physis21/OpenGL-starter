set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(OpenGL-starter main.cpp)
target_link_libraries(OpenGL-starter
    PRIVATE
        glad
        glfw
        GLErrorManager 
        VertexBuffer
        IndexBuffer
        VertexArray
        Shader
        Renderer
        Texture
        stb_image
)

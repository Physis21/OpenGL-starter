# To use the new OpenGL, and not compatibility version
cmake_policy(SET CMP0072 NEW)

add_library(RenderEngine)

target_sources(RenderEngine
    PRIVATE
        GLErrorManager.cpp
        VertexBuffer.cpp
        IndexBuffer.cpp
        VertexBufferLayout.cpp
        VertexArray.cpp
        Shader.cpp
        Renderer.cpp
        Texture.cpp
        ObjLoader.cpp
    PUBLIC FILE_SET HEADERS FILES
        GLErrorManager.hpp
        VertexBuffer.hpp
        IndexBuffer.hpp
        VertexBufferLayout.hpp
        VertexArray.hpp
        Shader.hpp
        Renderer.hpp
        Texture.hpp
        ObjLoader.hpp
)

target_link_libraries(RenderEngine
    glad
    stb_image
    glm::glm
    tinyobjloader
)

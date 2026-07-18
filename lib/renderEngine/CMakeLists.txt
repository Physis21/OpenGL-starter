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
        Camera.cpp
        Mesh.cpp
        Model.cpp
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
        Camera.hpp
        Mesh.hpp
        Model.hpp
)

target_link_libraries(RenderEngine
    glad
    glfw
    stb_image
    glm::glm
    tinyobjloader
    assimp
)

# PRIVATE means the PCH is used only when compiling RenderEngine's own cpp files.
# If I used PUBLIC, CMake would force these headers into the PCH of anything linking RenderEngine.
# For glad.h, use <glad.h> here instead of "glad.h" as CMake will otherwise try to resolve
# the file in the CMAKE_CURRENT_SOURCE_DIR. <..> searches in the compiler's include search.
target_precompile_headers(RenderEngine
    PRIVATE
        <vector>
        <string>
        <memory>
        <iostream>
        <unordered_map>
        <sstream>
        <fstream>
        <glad.h>
        <GLFW/glfw3.h>
        <glm/glm.hpp>
)

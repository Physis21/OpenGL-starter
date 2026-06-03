# To use the new OpenGL, and not compatibility version
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)


# GL Error Manager library
add_library(GLErrorManager)
target_sources(GLErrorManager
  PRIVATE GLErrorManager.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES GLErrorManager.hpp
)
target_link_libraries(GLErrorManager
  PRIVATE
    glad
    OpenGL::GL
)

# VertexBuffer class
add_library(VertexBuffer)
target_sources(VertexBuffer
  PRIVATE VertexBuffer.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES VertexBuffer.hpp
)
target_link_libraries(VertexBuffer
  PRIVATE
    GLErrorManager 
    glad
    OpenGL::GL
)

# IndexBuffer class
add_library(IndexBuffer)
target_sources(IndexBuffer
  PRIVATE IndexBuffer.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES IndexBuffer.hpp
)
target_link_libraries(IndexBuffer
  PRIVATE
    GLErrorManager 
    glad
    OpenGL::GL
)

# VertexBufferLayout class
add_library(VertexBufferLayout)
target_sources(VertexBufferLayout
  PRIVATE VertexBufferLayout.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES VertexBufferLayout.hpp
)
target_link_libraries(VertexBufferLayout
  PRIVATE
    GLErrorManager 
    glad
    OpenGL::GL
)

# VertexArray class, using VertexBufferLayout class
add_library(VertexArray)
target_sources(VertexArray
  PRIVATE VertexArray.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES VertexArray.hpp
)
target_link_libraries(VertexArray
  PRIVATE
    GLErrorManager 
    glad
    OpenGL::GL
    VertexBufferLayout
    VertexBuffer
)

# Shader class

add_library(Shader)
target_sources(Shader
  PRIVATE Shader.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES Shader.hpp
)
target_link_libraries(Shader
  PRIVATE
    GLErrorManager
    glad
    OpenGL::GL
)

# Renderer library
add_library(Renderer)
target_sources(Renderer
  PRIVATE Renderer.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES Renderer.hpp
)
target_link_libraries(Renderer
  PRIVATE
    glad
    OpenGL::GL
    GLErrorManager
    VertexArray
    IndexBuffer
    Shader
)

# Texture class
add_library(Texture)
target_sources(Texture
  PRIVATE Texture.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES Texture.hpp
)
target_link_libraries(Texture
  PRIVATE
    glad
    OpenGL::GL
    GLErrorManager
    stb_image
)

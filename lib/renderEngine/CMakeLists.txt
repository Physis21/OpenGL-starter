# To use the new OpenGL, and not compatibility version
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

# Renderer library

add_library(renderer)

target_sources(renderer
  PRIVATE Renderer.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES Renderer.hpp
)

target_link_libraries(renderer
  PUBLIC
    glad
    OpenGL::GL
)

# VertexBuffer class

add_library(vertexBuffer)

target_sources(vertexBuffer
  PRIVATE VertexBuffer.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES VertexBuffer.hpp
)

target_link_libraries(vertexBuffer
  PRIVATE
    renderer
    glad
    OpenGL::GL
)

# IndexBuffer class

add_library(indexBuffer)

target_sources(indexBuffer
  PRIVATE IndexBuffer.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES IndexBuffer.hpp
)

target_link_libraries(indexBuffer
  PRIVATE
    renderer
    glad
    OpenGL::GL
)

add_library(vertexBufferLayout)

target_sources(vertexBufferLayout
  PRIVATE VertexBufferLayout.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES VertexBufferLayout.hpp
)

target_link_libraries(vertexBufferLayout
  PRIVATE
    renderer
    glad
    OpenGL::GL
)

# VertexArray class, with VertexBufferLayout header-only library

add_library(vertexArray)

target_sources(vertexArray
  PRIVATE VertexArray.cpp
  PUBLIC
    FILE_SET HEADERS
    FILES VertexArray.hpp
)

target_link_libraries(vertexArray
  PRIVATE
    renderer
    glad
    OpenGL::GL
    vertexBufferLayout
    vertexBuffer
)


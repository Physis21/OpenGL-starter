#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include <glad.h>

class Renderer {
    public:
        /* Choose a clearColor and call `glClear()` with it*/
        void Clear(GLfloat color[4]) const;
        /* Draw an object with filled polygons */
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        /* Render an object as a wireframe. The object should not use a texture, preferably. */
        void DrawWireFrame(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

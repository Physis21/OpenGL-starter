#include "Renderer.hpp"
#include "GLErrorManager.hpp"
#include "glad.h"
#include <GL/gl.h>

void Renderer::Clear() const{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    // Might use unsigned short instead of unsigned int for performance reasons, but not considered here.
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

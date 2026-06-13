#include "Renderer.hpp"
#include "GLErrorManager.hpp"
#include "glad.h"

void Renderer::Clear(GLfloat color[4]) const{
    GLCall(glClearColor(color[0], color[1], color[2], color[3]));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    // Fill the front and back of polygons.
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    // Might use unsigned short instead of unsigned int for performance reasons, but not considered here.
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawWireFrame(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    // Use OpenGL's built in wireframe mode. Much easier than using GL_LINES
    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    // Might use unsigned short instead of unsigned int for performance reasons, but not considered here.
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

#pragma once

#include "GLErrorManager.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "glad.h"

class VertexArray {

private:
    unsigned int m_RendererID;


public:
    VertexArray();
    ~VertexArray();

    /* Bind the vertex array, and bind the vertex buffer to it.
     * The VertexBufferElements of the VertexBufferLayout are iterated on,
     * and each is attributed an spec with `glVertexAttribPointer`. */
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

};

#pragma once

#include "GLErrorManager.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include <glad.h>

class VertexArray {

private:
    unsigned int m_RendererID;


public:
    VertexArray();
    ~VertexArray();
    
    // Disallow copying, because destructor deletes EBO in memory
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept
        : m_RendererID(other.m_RendererID) {
            other.m_RendererID = 0; // null-out so its destructor is a no-op
        }
    VertexArray& operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            GLCall(glDeleteVertexArrays(1, &m_RendererID)); // free what we currently own
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

    /* Bind the vertex array, and bind the vertex buffer to it.
     * The VertexBufferElements of the VertexBufferLayout are iterated on,
     * and each is attributed an spec with `glVertexAttribPointer`. */
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

};

#pragma once

#include "GLErrorManager.hpp"
#include "glad.h"

class VertexBuffer {

private:
    unsigned int m_RendererID;

public:
    /*
     * param[in] data the vertex buffer data
     * param[in] size the size of `data`, **IN BYTES**.
     * assigns a value to `m_RendererID` after binding a buffer
     */
    VertexBuffer(const void* data, const unsigned int& size);
    ~VertexBuffer();

    // Disallow copying, because destructor deletes VBO in memory
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept
        : m_RendererID(other.m_RendererID) {
            other.m_RendererID = 0; // null-out so its destructor is a no-op
        }
    VertexBuffer& operator=(VertexBuffer&& other) noexcept {
        if (this != &other) {
            GLCall(glDeleteBuffers(1, &m_RendererID)) ;
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

    void Bind() const;
    void Unbind() const;
};

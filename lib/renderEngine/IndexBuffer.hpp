#pragma once

#include "GLErrorManager.hpp"
#include "glad.h"

class IndexBuffer {

private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    /*
     * param[in] data the indices of each vertex for each OpenGL geometry
     * param[in] count the number of indices (6 for a cube)
     * Assigns a value to `m_RendererID` and `m_Count`
     */
    IndexBuffer(const unsigned int* data, const unsigned int& count);
    ~IndexBuffer();
    
    // Disallow copying, because destructor deletes EBO in memory
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept
        : m_RendererID(other.m_RendererID) {
            other.m_RendererID = 0; // null-out so its destructor is a no-op
        }
    IndexBuffer& operator=(IndexBuffer&& other) noexcept {
        if (this != &other) {
            GLCall(glDeleteBuffers(1, &m_RendererID)); // free what we currently own
            m_RendererID = other.m_RendererID;
            other.m_RendererID = 0;
        }
        return *this;
    }

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const {return m_Count;};
};

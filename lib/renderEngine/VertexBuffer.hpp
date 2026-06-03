#pragma once

#include "GLErrorManager.hpp"
#include "glad.h"

class VertexBuffer {

private:
    unsigned int m_RendererID;

public:
    /*
     * param[in] data the vertex buffer data
     * param[in] size the size of `data`
     * assigns a value to `m_RendererID` after binding a buffer
     */
    VertexBuffer(const void* data, const unsigned int& size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;
};

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

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const {return m_Count;};
};

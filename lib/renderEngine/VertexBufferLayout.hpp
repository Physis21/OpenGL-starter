#pragma once

#include "Renderer.hpp"
#include "glad.h"
#include <vector>

/*
 * Charaterizes the type, count, and normalization value of a vertex buffer element
 * Thus, this struct does not require the actual vertex buffer
 */
struct VertexBufferElement {
    unsigned int type; // OpenGL type of each component in the array
    unsigned int count; // Number of components per generic vertex attribute
    unsigned char normalized; // Whether the fixed-point data values should be normalized when accessed

    static unsigned int GetSizeOfType(const unsigned int& type);
};

class VertexBufferLayout {

private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout() : m_Stride(0) {};

    template<typename T>
    void Push(const unsigned int& count);


    inline const std::vector<VertexBufferElement> GetElements() const {return m_Elements;};
    inline unsigned int GetStride() const {return m_Stride;};

};

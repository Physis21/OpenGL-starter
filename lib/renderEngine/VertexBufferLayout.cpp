#include "VertexBufferLayout.hpp"

/*
 * @param[in] The OpenGL type of the vertex buffer element
 * @returns the size of the OpenGL type
 */
unsigned int VertexBufferElement::GetSizeOfType(const unsigned int& type) {
    switch(type) {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
    }
    ASSERT(false); // Return false if the type doesn't correspond to the switch options
    return 0;
}

/*
 * Adds a vertex buffer element to the vertex buffer layout,
 * and increments the stride with the buffer element size.
 * @param[in] count the number of vertex buffer layouts to push
 */
template<typename T>
void VertexBufferLayout::Push(const unsigned int& count) {
    ASSERT(false);
}


template<>
void VertexBufferLayout::Push<float>(const unsigned int& count) {
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE }); // not normalized
    m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned int>(const unsigned int& count) {
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
void VertexBufferLayout::Push<unsigned char>(const unsigned int& count) {
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE }); // Normalized
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

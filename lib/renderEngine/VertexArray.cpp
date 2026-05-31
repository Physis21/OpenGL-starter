#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "glad.h"

VertexArray::VertexArray() {
    MyGLCall(glGenVertexArrays(1, &m_RendererID));
};
VertexArray::~VertexArray() {
    MyGLCall(glDeleteVertexArrays(1, &m_RendererID));
};

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {

    /* Bind the vertex array, and bind the bertex buffer to it */
    Bind();
    vb.Bind();

    /* Setup the layout for the vertex array */
    const auto& elements = layout.GetElements();
    unsigned long long int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const VertexBufferElement& element = elements[i];
        /* i is the index of the generic vertex attribute in the VAO to be enabled or disabled 
         * This line of code links the buffer to the VAO. */
        MyGLCall(glEnableVertexAttribArray(i));
        MyGLCall(glVertexAttribPointer(
                i,
                element.count, // Number of components per generic vertex attribute
                element.type,
                element.normalized,
                layout.GetStride(),
                (const void*)offset
        ));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
};

void VertexArray::Bind() const {
    MyGLCall(glBindVertexArray(m_RendererID));
};
void VertexArray::Unbind() const {
    MyGLCall(glBindVertexArray(0));
};


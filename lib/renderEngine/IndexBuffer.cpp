#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int& count)
: m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint)); // Checking for paranoids
    GLCall(glGenBuffers(1, &m_RendererID));
    /* Select a buffer (binding)
    GL_ARRAY_BUFFER tells the GPU this is just a buffer of memory */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    /* Buffer size should be in bytes, GL_STATIC_DRAW is a hint to the GL implementation */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    
};

IndexBuffer::~IndexBuffer(){
   GLCall(glDeleteBuffers(1, &m_RendererID)) ;
};

void IndexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
};
void IndexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
};

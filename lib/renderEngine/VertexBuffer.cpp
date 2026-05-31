#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, const unsigned int &size){
    MyGLCall(glGenBuffers(1, &m_RendererID)); // assign a value to m_RendererID
    /* Select a buffer (binding)
    GL_ARRAY_BUFFER tells the GPU this is just a buffer of memory */
    MyGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    /* Buffer size should be in bytes, GL_STATIC_DRAW is a hint to the GL implementation */
    MyGLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
    
};

VertexBuffer::~VertexBuffer(){
   MyGLCall(glDeleteBuffers(1, &m_RendererID)) ;
};

void VertexBuffer::Bind() const{
    MyGLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
};
void VertexBuffer::Unbind() const{
    MyGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
};

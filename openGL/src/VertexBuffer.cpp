#include "VertexBuffer.h"
#include "Renderer.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool Dynamic)
{
    GLCall(glGenBuffers(1, &m_RendererID));//创建缓冲区
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//绑定缓冲区到ARRAY_BUFFER
    if (!Dynamic){
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//将缓冲区数据写入
    }
    else {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));//将缓冲区数据写入
    }
}
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
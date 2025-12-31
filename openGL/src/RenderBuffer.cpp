#include "RenderBuffer.h"
#include "Renderer.h"
#include "config.h"

RenderBuffer::RenderBuffer()
{
	GLCall(glGenRenderbuffers(1, &m_RendererID));
	glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RendererID));
}

RenderBuffer::~RenderBuffer()
{
	GLCall(glDeleteRenderbuffers(1, &m_RendererID))
}

void RenderBuffer::Bind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID));
}
void RenderBuffer::Unbind() const
{
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
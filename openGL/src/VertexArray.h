#pragma once
#include "glm/glm.hpp"
class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddInstanceBuffer(const VertexBuffer& vb);

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
	unsigned int m_AttribIndex = 0;
};
#pragma once

class RenderBuffer
{
private:
	unsigned int m_RendererID;
public:
	RenderBuffer();
	~RenderBuffer();

	void Check();

	void Bind() const;
	void Unbind() const;
};
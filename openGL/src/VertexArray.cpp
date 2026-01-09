#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID))
}

/// <summary>
/// 普通顶点缓冲区添加到顶点数组对象中
/// </summary>
/// <param name="vb"></param>
/// <param name="layout"></param>
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));//启动指定索引的顶点属性数组（glVertexAttribPointer）
		//规定传入数据的读取规则，也就是数组含义（坐标、纹理等）,实际上是将vbo绑定到vao
		GLCall(glVertexAttribPointer(m_AttribIndex, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		m_AttribIndex++;
	}

}

/// <summary>
/// 实例化矩阵缓冲区添加到顶点数组对象中
/// </summary>
/// <param name="vbo"></param>
/// <param name="startLocation"></param>
void VertexArray::AddInstanceBuffer(const VertexBuffer& vb)
{
    Bind();
	vb.Bind();

    std::size_t vec4Size = sizeof(glm::vec4);

    for (unsigned int i = 0; i < 4; i++)
    {
        glEnableVertexAttribArray(m_AttribIndex);
        glVertexAttribPointer(
			m_AttribIndex,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(glm::mat4),
            (void*)(i * vec4Size)
        );
        glVertexAttribDivisor(m_AttribIndex, 1);
		m_AttribIndex++;
    }
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
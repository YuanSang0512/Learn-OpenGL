#include "Mesh.h"
#include <iostream>
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures, std::vector<glm::mat4> instanceMatrices, RendererType type)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->instanceMatrices = instanceMatrices;
	this->type = type;
	//std::cout << vertices.size() << " vertices, " << indices.size() << " indices, " << textures.size() << " textures." << std::endl;
	setupMesh();
}

void Mesh::Draw(Shader& shader, RendererType m_Type, unsigned int instanceCount)
{
	Renderer renderer;
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.SetUniform1i(("u_Material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    if(m_Type == RendererType::Multiple)
        renderer.DrawInstance(*m_VAO, *m_EBO, shader, instanceCount);
    else
        renderer.Draw(*m_VAO, *m_EBO, shader);
	m_VAO->Unbind();
}

//注意VBO传入数据为nullptr
void Mesh::setupMesh()
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO_Normal = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex), false);
    m_EBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());

	m_VAO->Bind();
    VertexBufferLayout layout;
    layout.Push<float>(3);//Position
    layout.Push<float>(3);//Normal
    layout.Push<float>(2);//TexCoord
	m_VAO->AddBuffer(*m_VBO_Normal, layout);

    // 实例化矩阵 VBO
    if (type == RendererType::Multiple)
    {
        if (instanceMatrices.empty())
            std::cout << "Error: instanceMatrices empty!" << std::endl;
        else
        {
            m_VBO_Instance = std::make_unique<VertexBuffer>(instanceMatrices.data(), instanceMatrices.size() * sizeof(glm::mat4), false);
            // 绑定到 VAO 上，假设顶点属性 0/1/2 已经占用
            m_VAO->AddInstanceBuffer(*m_VBO_Instance);
        }
    }
}

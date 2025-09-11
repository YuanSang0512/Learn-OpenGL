#include "Mesh.h"
#include <iostream>
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	//std::cout << vertices.size() << " vertices, " << indices.size() << " indices, " << textures.size() << " textures." << std::endl;
	setupMesh();
}

void Mesh::Draw(Shader& shader)
{
	Renderer renderer;
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
        // ��ȡ������ţ�diffuse_textureN �е� N��
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

    // ��������
	renderer.Draw(*m_VAO, *m_EBO, shader);
	//m_VAO->Unbind();
}

//ע��VBO��������Ϊnullptr
void Mesh::setupMesh()
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex), false);
    m_EBO = std::make_unique<IndexBuffer>(indices.data(), indices.size());

	m_VAO->Bind();
    VertexBufferLayout layout;
    layout.Push<float>(3);//Position
    layout.Push<float>(3);//Normal
    layout.Push<float>(2);//TexCoord
    m_VAO->AddBuffer(*m_VBO, layout);

	//m_VAO->Unbind();
}

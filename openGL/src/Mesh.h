#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <memory>

#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "config.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct TextureInfo {
	unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    RendererType type = RendererType::Single;
    std::vector<Vertex> vertices;
	std::vector<glm::mat4> instanceMatrices;
    std::vector<unsigned int> indices;
    std::vector<TextureInfo> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures, std::vector<glm::mat4> instanceMatrices, RendererType type = RendererType::Single);
    void Draw(Shader& shader, RendererType m_Type, unsigned int instanceCount);
private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO_Normal;
    std::unique_ptr<VertexBuffer> m_VBO_Instance;
    std::unique_ptr<IndexBuffer> m_EBO;

    void setupMesh();
};
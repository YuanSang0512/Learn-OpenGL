#pragma once
#include <vector>
#include <string>

#include <Assimp/assimp/Importer.hpp>
#include <Assimp/assimp/scene.h>
#include <Assimp/assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "config.h"


class Model
{
public:
	Model() {}
    Model(const char* path, RendererType type = RendererType::Single, std::vector<glm::mat4> instanceMatrices = std::vector<glm::mat4>())
    {
        this->instanceMatrices = instanceMatrices;
		m_Type = type;
        loadModel(path);
    }
    void SetInstanceMatrices(std::vector<glm::mat4> matrices) { instanceMatrices = matrices; }
    void Draw(Shader& shader, std::vector<glm::mat4> instanceMatrix = std::vector<glm::mat4>());
    std::vector<Mesh> meshes;
private:
    /* 渲染模式 */
	RendererType m_Type;
    /*  模型数据  */
    std::string directory;
    std::vector<TextureInfo> textures_loaded;
	std::vector<glm::mat4> instanceMatrices;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
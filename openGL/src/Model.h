#pragma once
#include <vector>
#include <string>

#include <Assimp/assimp/Importer.hpp>
#include <Assimp/assimp/scene.h>
#include <Assimp/assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    /*  函数   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);
private:
    /*  模型数据  */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<TextureInfo> textures_loaded;
    /*  函数   */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);
};
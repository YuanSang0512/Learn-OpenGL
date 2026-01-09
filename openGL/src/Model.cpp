#include "Model.h"
#include "vendor/std_image/stb_image.h"

#include <iostream>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

void Model::Draw(Shader& shader, std::vector<glm::mat4> instanceMatrix)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, m_Type, instanceMatrix.size());
}

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
	std::cout << "Load " << meshes.size() << " meshes from model " << path << std::endl;
}

/// <summary>
/// 递归处理场景中的节点及其所有子节点，将每个节点的网格添加到模型中。
/// </summary>
/// <param name="node">要处理的当前节点，包含网格和子节点信息。</param>
/// <param name="scene">包含所有网格和场景数据的场景对象。</param>
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

/// <summary>
/// 处理给定的 aiMesh 和 aiScene，提取顶点、索引和纹理信息，并构建 Mesh 对象。用于将 Assimp 加载的网格数据转换为自定义 Mesh 结构，便于后续渲染和处理。
/// </summary>
/// <param name="mesh">指向 aiMesh 的指针，表示要处理的网格数据。</param>
/// <param name="scene">指向 aiScene 的指针，包含与网格相关的场景和材质信息。</param>
/// <returns>返回一个 Mesh 对象，包含处理后的顶点、索引和纹理信息。</returns>
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureInfo> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) // 网格是否包含纹理坐标？
			vertex.TexCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // 处理材质
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TextureInfo> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<TextureInfo> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
	//std::cout << "Load " << textures.size() << " textures from mesh." << std::endl;
	std::cout << "--------------------------------" << std::endl;
    std::cout << "Mesh vertices: " << vertices.size()
        << ", indices: " << indices.size()
        << ", textures: " << textures.size() << std::endl;
    std::cout << "--------------------------------" << std::endl;

    return Mesh(vertices, indices, textures, instanceMatrices, m_Type);
}

/// <summary>
/// 加载指定类型的材质纹理，并返回纹理信息列表。
/// </summary>
/// <param name="mat">指向 aiMaterial 对象的指针，表示要加载纹理的材质。</param>
/// <param name="type">要加载的纹理类型（aiTextureType 枚举值）。</param>
/// <param name="typeName">纹理类型的名称字符串。</param>
/// <returns>包含所有加载的纹理信息的 std::vector<TextureInfo>。</returns>
std::vector<TextureInfo> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<TextureInfo> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
		bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
		}
        if (!skip)
        {
            TextureInfo texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory, false);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
			textures_loaded.push_back(texture);
        }
    }
    return textures;
}

/// <summary>
/// 从文件加载一张纹理并返回其 OpenGL 纹理 ID。支持 gamma 校正选项。若加载失败，返回的纹理 ID 可能无效。需要依赖 OpenGL 和 stb_image 库。
/// </summary>
/// <param name="path">要加载的纹理文件的相对路径（C 字符串）。</param>
/// <param name="directory">包含纹理文件的目录（std::string 类型）。</param>
/// <param name="gamma">是否启用 gamma 校正（布尔值）。</param>
/// <returns>加载的 OpenGL 纹理对象的 ID（无符号整型）。</returns>
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    stbi_set_flip_vertically_on_load(1);//把图片上下翻转

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "================================" << path << std::endl;
        std::cout << "Texture failed to load at path: " << path << std::endl;
        std::cout << "================================" << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
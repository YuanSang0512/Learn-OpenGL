#include "Model.h"
#include "vendor/std_image/stb_image.h"

#include <iostream>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

void Model::Draw(Shader& shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
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
}

/// <summary>
/// �ݹ鴦�����еĽڵ㼰�������ӽڵ㣬��ÿ���ڵ��������ӵ�ģ���С�
/// </summary>
/// <param name="node">Ҫ����ĵ�ǰ�ڵ㣬����������ӽڵ���Ϣ��</param>
/// <param name="scene">������������ͳ������ݵĳ�������</param>
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // ����ڵ����е���������еĻ���
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // �������������ӽڵ��ظ���һ����
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

/// <summary>
/// ��������� aiMesh �� aiScene����ȡ���㡢������������Ϣ�������� Mesh �������ڽ� Assimp ���ص���������ת��Ϊ�Զ��� Mesh �ṹ�����ں�����Ⱦ�ʹ���
/// </summary>
/// <param name="mesh">ָ�� aiMesh ��ָ�룬��ʾҪ������������ݡ�</param>
/// <param name="scene">ָ�� aiScene ��ָ�룬������������صĳ����Ͳ�����Ϣ��</param>
/// <returns>����һ�� Mesh ���󣬰��������Ķ��㡢������������Ϣ��</returns>
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureInfo> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        // ������λ�á����ߺ���������
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (mesh->mTextureCoords[0]) // �����Ƿ�����������ꣿ
			vertex.TexCoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		else
			vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // ��������
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // �������
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

    return Mesh(vertices, indices, textures);
}

/// <summary>
/// ����ָ�����͵Ĳ�������������������Ϣ�б�
/// </summary>
/// <param name="mat">ָ�� aiMaterial �����ָ�룬��ʾҪ��������Ĳ��ʡ�</param>
/// <param name="type">Ҫ���ص��������ͣ�aiTextureType ö��ֵ����</param>
/// <param name="typeName">�������͵������ַ�����</param>
/// <returns>�������м��ص�������Ϣ�� std::vector<TextureInfo>��</returns>
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
/// ���ļ�����һ������������ OpenGL ���� ID��֧�� gamma У��ѡ�������ʧ�ܣ����ص����� ID ������Ч����Ҫ���� OpenGL �� stb_image �⡣
/// </summary>
/// <param name="path">Ҫ���ص������ļ������·����C �ַ�������</param>
/// <param name="directory">���������ļ���Ŀ¼��std::string ���ͣ���</param>
/// <param name="gamma">�Ƿ����� gamma У��������ֵ����</param>
/// <returns>���ص� OpenGL �������� ID���޷������ͣ���</returns>
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

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
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
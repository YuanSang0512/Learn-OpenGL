#include "BasicModels.h"

std::array<BasicModel::Vertex, 24> BasicModel::BasicModels::CreateCubeVertexs(float size)
{
    std::array<Vertex, 24> vertices;
    Vertex* target = vertices.data();
    float r = size / 2.0f;

    vec2 uv0 = { 0.0f, 0.0f };
    vec2 uv1 = { 1.0f, 0.0f };
    vec2 uv2 = { 1.0f, 1.0f };
    vec2 uv3 = { 0.0f, 1.0f };

    // Front face (+Z)
    vec3 frontNormal = { 0.0f, 0.0f, 1.0f };
    target->position = { r,  r, -r }; target->normal = frontNormal; target->texCoord = uv2; target++;
    target->position = { -r,  r, -r }; target->normal = frontNormal; target->texCoord = uv3; target++;
    target->position = { -r, -r, -r }; target->normal = frontNormal; target->texCoord = uv0; target++;
    target->position = { r, -r, -r }; target->normal = frontNormal; target->texCoord = uv1; target++;

    // Back face (-Z)
    vec3 backNormal = { 0.0f, 0.0f, -1.0f };
    target->position = { r,  r,  r }; target->normal = backNormal; target->texCoord = uv2; target++;
    target->position = { -r,  r,  r }; target->normal = backNormal; target->texCoord = uv3; target++;
    target->position = { -r, -r,  r }; target->normal = backNormal; target->texCoord = uv0; target++;
    target->position = { r, -r,  r }; target->normal = backNormal; target->texCoord = uv1; target++;

    // Right face (+X)
    vec3 rightNormal = { 1.0f, 0.0f, 0.0f };
    target->position = { r,  r, -r }; target->normal = rightNormal; target->texCoord = uv2; target++;
    target->position = { r,  r,  r }; target->normal = rightNormal; target->texCoord = uv3; target++;
    target->position = { r, -r,  r }; target->normal = rightNormal; target->texCoord = uv0; target++;
    target->position = { r, -r, -r }; target->normal = rightNormal; target->texCoord = uv1; target++;

    // Left face (-X)
    vec3 leftNormal = { -1.0f, 0.0f, 0.0f };
    target->position = { -r,  r, -r }; target->normal = leftNormal; target->texCoord = uv2; target++;
    target->position = { -r,  r,  r }; target->normal = leftNormal; target->texCoord = uv3; target++;
    target->position = { -r, -r,  r }; target->normal = leftNormal; target->texCoord = uv0; target++;
    target->position = { -r, -r, -r }; target->normal = leftNormal; target->texCoord = uv1; target++;

    // Top face (+Y)
    vec3 topNormal = { 0.0f, 1.0f, 0.0f };
    target->position = { r,  r, -r }; target->normal = topNormal; target->texCoord = uv2; target++;
    target->position = { -r,  r, -r }; target->normal = topNormal; target->texCoord = uv3; target++;
    target->position = { -r,  r,  r }; target->normal = topNormal; target->texCoord = uv0; target++;
    target->position = { r,  r,  r }; target->normal = topNormal; target->texCoord = uv1; target++;

    // Bottom face (-Y)
    vec3 bottomNormal = { 0.0f, -1.0f, 0.0f };
    target->position = { r, -r, -r }; target->normal = bottomNormal; target->texCoord = uv2; target++;
    target->position = { -r, -r, -r }; target->normal = bottomNormal; target->texCoord = uv3; target++;
    target->position = { -r, -r,  r }; target->normal = bottomNormal; target->texCoord = uv0; target++;
    target->position = { r, -r,  r }; target->normal = bottomNormal; target->texCoord = uv1; target++;

    return vertices;
}
std::vector<unsigned int> BasicModel::BasicModels::CreateCubeIndices()
{
    std::vector<unsigned int> indices(36, 0);
    int offset = 0;

    // Front face
    indices[0] = 0 + offset; indices[1] = 1 + offset; indices[2] = 2 + offset;
    indices[3] = 2 + offset; indices[4] = 3 + offset; indices[5] = 0 + offset;

    // Back face
    indices[6] = 4 + offset; indices[7] = 5 + offset; indices[8] = 6 + offset;
    indices[9] = 6 + offset; indices[10] = 7 + offset; indices[11] = 4 + offset;

    // Right face
    indices[12] = 8 + offset; indices[13] = 9 + offset; indices[14] = 10 + offset;
    indices[15] = 10 + offset; indices[16] = 11 + offset; indices[17] = 8 + offset;

    // Left face
    indices[18] = 12 + offset; indices[19] = 13 + offset; indices[20] = 14 + offset;
    indices[21] = 14 + offset; indices[22] = 15 + offset; indices[23] = 12 + offset;

    // Top face
    indices[24] = 16 + offset; indices[25] = 17 + offset; indices[26] = 18 + offset;
    indices[27] = 18 + offset; indices[28] = 19 + offset; indices[29] = 16 + offset;

    // Bottom face
    indices[30] = 20 + offset; indices[31] = 21 + offset; indices[32] = 22 + offset;
    indices[33] = 22 + offset; indices[34] = 23 + offset; indices[35] = 20 + offset;

    return indices;
}

std::array<BasicModel::skyVertex, 24> BasicModel::BasicModels::CreateSkyBoxVertexs()
{
    std::array<skyVertex, 24> vertices;
    skyVertex* target = vertices.data();

    // Front face (+Z)
    target->position = { -1,  1,  1 }; target++;
    target->position = { -1, -1,  1 }; target++;
    target->position = {  1, -1,  1 }; target++;
    target->position = {  1,  1,  1 }; target++;

    // Back face (-Z)
    target->position = {  1,  1, -1 }; target++;
    target->position = {  1, -1, -1 }; target++;
    target->position = { -1, -1, -1 }; target++;
    target->position = { -1,  1, -1 }; target++;

    // Right face (+X)
    target->position = { 1,  1,  1 }; target++;
    target->position = { 1, -1,  1 }; target++;
    target->position = { 1, -1, -1 }; target++;
    target->position = { 1,  1, -1 }; target++;

    // Left face (-X)
    target->position = { -1,  1, -1 }; target++;
    target->position = { -1, -1, -1 }; target++;
    target->position = { -1, -1,  1 }; target++;
    target->position = { -1,  1,  1 }; target++;

    // Top face (+Y)
    target->position = { -1,  1,  1 }; target++;
    target->position = { -1,  1, -1 }; target++;
    target->position = {  1,  1, -1 }; target++;
    target->position = {  1,  1,  1 }; target++;

    // Bottom face (-Y)
    target->position = { -1, -1, -1 }; target++;
    target->position = { -1, -1,  1 }; target++;
    target->position = {  1, -1,  1 }; target++;
    target->position = {  1, -1, -1 }; target++;

    return vertices;
}
std::vector<unsigned int> BasicModel::BasicModels::CreateSkyBoxIndices()
{
    std::vector<unsigned int> indices(36, 0);
    int offset = 0;

    // Front face
    indices[0] = 0 + offset; indices[1] = 1 + offset; indices[2] = 2 + offset;
    indices[3] = 2 + offset; indices[4] = 3 + offset; indices[5] = 0 + offset;

    // Back face
    indices[6] = 4 + offset; indices[7] = 5 + offset; indices[8] = 6 + offset;
    indices[9] = 6 + offset; indices[10] = 7 + offset; indices[11] = 4 + offset;

    // Right face
    indices[12] = 8 + offset; indices[13] = 9 + offset; indices[14] = 10 + offset;
    indices[15] = 10 + offset; indices[16] = 11 + offset; indices[17] = 8 + offset;

    // Left face
    indices[18] = 12 + offset; indices[19] = 13 + offset; indices[20] = 14 + offset;
    indices[21] = 14 + offset; indices[22] = 15 + offset; indices[23] = 12 + offset;

    // Top face
    indices[24] = 16 + offset; indices[25] = 17 + offset; indices[26] = 18 + offset;
    indices[27] = 18 + offset; indices[28] = 19 + offset; indices[29] = 16 + offset;

    // Bottom face
    indices[30] = 20 + offset; indices[31] = 21 + offset; indices[32] = 22 + offset;
    indices[33] = 22 + offset; indices[34] = 23 + offset; indices[35] = 20 + offset;

    return indices;
}

std::array<BasicModel::Vertex, 4> BasicModel::BasicModels::CreatePlaneVertexs(vec2 size, vec3 normal)
{
    std::array<Vertex, 4> vertices;
    Vertex* target = vertices.data(); 
    float hx = size.x / 2.0f;
    float hy = size.y / 2.0f;

   vec2 uv0 = { 0.0f, 0.0f };
   vec2 uv1 = { 1.0f, 0.0f };
   vec2 uv2 = { 1.0f, 1.0f };
   vec2 uv3 = { 0.0f, 1.0f };

    // 判断 normal 朝向，生成对应平面
    if (normal.x != 0.0f) // YZ 平面
    {
        float dir = (normal.x > 0) ? 1.0f : -1.0f;
        target->position = { dir * 0.0f,  hy, -hx }; target->normal = normal; target->texCoord = uv2; target++;
        target->position = { dir * 0.0f,  hy,  hx }; target->normal = normal; target->texCoord = uv3; target++;
        target->position = { dir * 0.0f, -hy,  hx }; target->normal = normal; target->texCoord = uv0; target++;
        target->position = { dir * 0.0f, -hy, -hx }; target->normal = normal; target->texCoord = uv1; target++;
    }
    else if (normal.y != 0.0f) // XZ 平面
    {
        float dir = (normal.y > 0) ? 1.0f : -1.0f;
        target->position = { hx, dir * 0.0f, -hy }; target->normal = normal; target->texCoord = uv2; target++;
        target->position = { -hx, dir * 0.0f, -hy }; target->normal = normal; target->texCoord = uv3; target++;
        target->position = { -hx, dir * 0.0f,  hy }; target->normal = normal; target->texCoord = uv0; target++;
        target->position = { hx, dir * 0.0f,  hy }; target->normal = normal; target->texCoord = uv1; target++;
    }
    else // XY 平面 (normal z != 0)
    {
        float dir = (normal.z > 0) ? 1.0f : -1.0f;
        target->position = { hx,  hy, dir * 0.0f }; target->normal = normal; target->texCoord = uv2; target++;
        target->position = { -hx,  hy, dir * 0.0f }; target->normal = normal; target->texCoord = uv3; target++;
        target->position = { -hx, -hy, dir * 0.0f }; target->normal = normal; target->texCoord = uv0; target++;
        target->position = { hx, -hy, dir * 0.0f }; target->normal = normal; target->texCoord = uv1; target++;
    }

    return vertices;
}
std::vector<unsigned int> BasicModel::BasicModels::CreatePlaneIndices()
{
    std::vector<unsigned int> indices(6, 0);
    int offset = 0;

    indices[0] = 0 + offset; indices[1] = 1 + offset; indices[2] = 2 + offset;
    indices[3] = 2 + offset; indices[4] = 3 + offset; indices[5] = 0 + offset;

    return indices;
}

std::vector<float> BasicModel::BasicModels::GetScrVertex()
{
    std::vector<float> quadVertices = {
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
    };
    return quadVertices;
}
#include "BasicModels.h"

vertex* BasicModels::CreateCubeVertexs(vertex* target, float size)
{
    float r = size / 2.0f;

    // 常用纹理坐标
    vec2 uv0 = { 0.0f, 0.0f };
    vec2 uv1 = { 1.0f, 0.0f };
    vec2 uv2 = { 1.0f, 1.0f };
    vec2 uv3 = { 0.0f, 1.0f };

    // Front face (+Z)
    vec3 frontNormal = { 0.0f, 0.0f, -1.0f };
    target->position = { r,  r, -r }; target->normal = frontNormal; target->texCoord = uv2; target++;
    target->position = { -r,  r, -r }; target->normal = frontNormal; target->texCoord = uv3; target++;
    target->position = { -r, -r, -r }; target->normal = frontNormal; target->texCoord = uv0; target++;
    target->position = { r, -r, -r }; target->normal = frontNormal; target->texCoord = uv1; target++;

    // Back face (-Z)
    vec3 backNormal = { 0.0f, 0.0f, 1.0f };
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

    return target;
}

std::vector<unsigned int> BasicModels::CreateCubeIndices(std::vector<unsigned int> indices)
{
    int offset = 0;
    for (int i = 0; i < indices.size(); i += 36)
    {
        // Front face
        indices[i + 0] = 0 + offset; indices[i + 1] = 1 + offset; indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset; indices[i + 4] = 3 + offset; indices[i + 5] = 0 + offset;

        // Back face
        indices[i + 6] = 4 + offset; indices[i + 7] = 5 + offset; indices[i + 8] = 6 + offset;
        indices[i + 9] = 6 + offset; indices[i + 10] = 7 + offset; indices[i + 11] = 4 + offset;

        // Right face
        indices[i + 12] = 8 + offset; indices[i + 13] = 9 + offset; indices[i + 14] = 10 + offset;
        indices[i + 15] = 10 + offset; indices[i + 16] = 11 + offset; indices[i + 17] = 8 + offset;

        // Left face
        indices[i + 18] = 12 + offset; indices[i + 19] = 13 + offset; indices[i + 20] = 14 + offset;
        indices[i + 21] = 14 + offset; indices[i + 22] = 15 + offset; indices[i + 23] = 12 + offset;

        // Top face
        indices[i + 24] = 16 + offset; indices[i + 25] = 17 + offset; indices[i + 26] = 18 + offset;
        indices[i + 27] = 18 + offset; indices[i + 28] = 19 + offset; indices[i + 29] = 16 + offset;

        // Bottom face
        indices[i + 30] = 20 + offset; indices[i + 31] = 21 + offset; indices[i + 32] = 22 + offset;
        indices[i + 33] = 22 + offset; indices[i + 34] = 23 + offset; indices[i + 35] = 20 + offset;

        offset += 24;
    }
    return indices;
}

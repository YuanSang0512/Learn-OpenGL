#pragma once
#include <vector>
struct vec2 {
    float x, y;
};

struct vec3 {
    float x, y, z;
};

struct vec4 {
    float x, y, z, w;
};

struct vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

struct skyVertex {
    vec3 position;
};

class BasicModels
{
public:
    vertex* CreateCubeVertexs(vertex* target, float size);
    std::vector<unsigned int> CreateCubeIndices(std::vector<unsigned int> indices);

    skyVertex* CreateSkyBoxVertexs(skyVertex* target, float size);
    std::vector<unsigned int> CreateSkyBoxIndices(std::vector<unsigned int> indices);

    vertex* CreatePlaneVertexs(vertex* target, vec2 size, vec3 normal);
    std::vector<unsigned int> CreatePlaneIndices(std::vector<unsigned int> indices);

};

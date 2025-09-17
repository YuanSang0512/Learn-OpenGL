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

class BasicModels
{
public:
    vertex* CreateCubeVertexs(vertex* target, float size);
    std::vector<unsigned int> CreateCubeIndices(std::vector<unsigned int> indices);

    std::vector<unsigned int> CreatePlaneIndices(std::vector<unsigned int> indices);
    vertex* CreatePlaneVertexs(vertex* target, vec2 size, vec3 normal);

};

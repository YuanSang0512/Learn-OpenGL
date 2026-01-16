#pragma once
#include <vector>
#include <array>
#include "glm/glm.hpp"

namespace BasicModel {
    struct vec2 {
        float x, y;

        vec2() : x(0), y(0) {}
        vec2(float _x, float _y) : x(_x), y(_y) {}
    };

    struct vec3 {
        float x, y, z;

        vec3() : x(0), y(0), z(0) {}
        vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    };

    struct vec4 {
        float x, y, z, w;
    };

    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 texCoord;
    };

    struct ScrVertex {
        vec2 position;
        vec2 texCoord;
    };

    struct skyVertex {
        vec3 position;
    };

    vec3 normalize(vec3& v);

    class BasicModels
    {
    public:
        //立方体（坐标：法线：纹理坐标）
        std::array<Vertex, 24> CreateCubeVertexs(float size);
        std::vector<unsigned int> CreateCubeIndices();

        //天空盒（坐标）
        std::array<skyVertex, 24> CreateSkyBoxVertexs();
        std::vector<unsigned int> CreateSkyBoxIndices();

        //平面（坐标：法线：纹理坐标）
        std::array<Vertex, 4> CreatePlaneVertexs(vec2 size, vec3 normal);
        std::vector<unsigned int> CreatePlaneIndices();

        //球体（坐标：法线：纹理坐标）
        std::vector<BasicModel::Vertex> CreateSphereVertices(float radius, unsigned int xSegments, unsigned int ySegments);
        std::vector<unsigned int> CreateSphereIndices(unsigned int xSegments, unsigned int ySegments);


        //屏幕平面（坐标：纹理坐标）
        std::vector<float> GetScrVertex();
    };
}

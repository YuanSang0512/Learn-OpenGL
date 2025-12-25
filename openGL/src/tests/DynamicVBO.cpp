#include "DynamicVBO.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>

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
    vec4 color;
    vec2 texCoord;
    float texIndex;
};

static std::array<vertex, 4> CreateQuad(float m_x, float m_y, float m_z, float m_texIndex)
{
    float size = 200.0f;
    vertex v0;
    v0.position = { m_x, m_y, m_z };
    v0.color = { 1.0f, 0.0f, 0.0f, 1.0f };
    v0.texCoord = { 0.0f, 0.0f };
    v0.texIndex = m_texIndex;

    vertex v1;
    v1.position = { m_x + size, m_y, m_z };
    v1.color = { 0.0f, 1.0f, 0.0f, 1.0f };
    v1.texCoord = { 1.0f, 0.0f };
    v1.texIndex = m_texIndex;

    vertex v2;
    v2.position = { m_x + size, m_y + size, m_z };
    v2.color = { 0.0f, 0.0f, 1.0f, 1.0f };
    v2.texCoord = { 1.0f, 1.0f };
    v2.texIndex = m_texIndex;

    vertex v3;
    v3.position = { m_x, m_y + size, m_z };
    v3.color = { 1.0f, 0.0f, 1.0f, 1.0f };
    v3.texCoord = { 0.0f, 1.0f };
    v3.texIndex = m_texIndex;

    return { v0, v1, v2, v3 };
}

namespace test
{

    DynamicVBO::DynamicVBO()
        : positionA(0, 0, 0), positionB(0, 0, 0)
    {
        //用于指定坐标读取顺序
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));//启用混合

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 1000*sizeof(vertex), true);//坐标缓冲对象
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));

        m_proj = glm::ortho(-900.0f, 900.0f, -900.0f, 900.0f, -900.0f, 900.0f);//正交投影 渲染的边界（上下左右前后）
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader = std::make_unique<Shader>("res/shaders/DynamicVBO.shader");
        m_Shader->Bind();

        m_Texture1 = std::make_unique<Texture>("res/textures/earth.png", TextureType::TEXTURE_2D);
        m_Texture2 = std::make_unique<Texture>("res/textures/maodie.jpg", TextureType::TEXTURE_2D);
    }

    DynamicVBO::~DynamicVBO()
    {

    }

    void DynamicVBO::OnUpdate(float deltaTime)
    {
        std::array<vertex, 4> q0 = CreateQuad(positionA.x, positionA.y, positionA.z, 0.0f);
        std::array<vertex, 4> q1 = CreateQuad(positionB.x, positionB.y, positionB.z, 1.0f);

        vertex vertices[8];
        memcpy(vertices, q0.data(), q0.size() * sizeof(vertex));
        memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(vertex));

        m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        layout.Push<float>(4);//Color
        layout.Push<float>(2);//TexCoord
        layout.Push<float>(1);//TextureIndex
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
    }

    void DynamicVBO::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer render;
        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

        int sampler[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, sampler);


        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0, 0, 0));
        glm::mat4 mvp = m_proj * m_view * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void DynamicVBO::OnImGuiRender()
    {
        ImGui::SliderFloat3("positionA", &positionA.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("positionB", &positionB.x, -900.0f, 900.0f);
    }
}
#include "VertexIndex.h"

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

static vertex* CreateQuad(vertex* target, float m_x, float m_y, float m_z, float m_texIndex)
{
    float size = 200.0f;
    target->position = { m_x, m_y, m_z };
    target->color = { 1.0f, 0.0f, 0.0f, 1.0f };
    target->texCoord = { 0.0f, 0.0f };
    target->texIndex = m_texIndex;
    target++;

    target->position = { m_x + size, m_y, m_z };
    target->color = { 0.0f, 1.0f, 0.0f, 1.0f };
    target->texCoord = { 1.0f, 0.0f };
    target->texIndex = m_texIndex;
    target++;

    target->position = { m_x + size, m_y + size, m_z };
    target->color = { 0.0f, 0.0f, 1.0f, 1.0f };
    target->texCoord = { 1.0f, 1.0f };
    target->texIndex = m_texIndex;
    target++;

    target->position = { m_x, m_y + size, m_z };
    target->color = { 1.0f, 0.0f, 1.0f, 1.0f };
    target->texCoord = { 0.0f, 1.0f };
    target->texIndex = m_texIndex;
    target++;

    return target;
}

namespace test
{
    const int MaxQuadCount = 2;
    const int MaxVertexCount = MaxQuadCount * 4;
    const int MaxIndexCount = MaxQuadCount * 6;

    VertexIndex::VertexIndex()
        : positionA(0, 0, 0), positionB(0, 0, 0)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));//启用混合


        unsigned int indices[MaxIndexCount];
        int offset = 0;
        for (int i = 0; i < MaxIndexCount; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;

            offset += 4;
        }

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(vertex), true);//坐标缓冲对象
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

        m_proj = glm::ortho(-900.0f, 900.0f, -900.0f, 900.0f, -900.0f, 900.0f);//正交投影 渲染的边界（上下左右前后）
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader = std::make_unique<Shader>("res/shaders/VertexIndex.shader");
        m_Shader->Bind();

        m_Texture1 = std::make_unique<Texture>("res/textures/earth.png", TEXTURE_2D);
        m_Texture2 = std::make_unique<Texture>("res/textures/maodie.jpg", TEXTURE_2D);
    }

    VertexIndex::~VertexIndex()
    {

    }

    void VertexIndex::OnUpdate(float deltaTime)
    {
        std::array<vertex, MaxVertexCount> vertices;
        vertex* buffer = vertices.data();

        buffer = CreateQuad(buffer, -900, -900, 0, 0);
        buffer = CreateQuad(buffer, -900, -700, 0, 1);

        m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());

        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        layout.Push<float>(4);//Color
        layout.Push<float>(2);//TexCoord
        layout.Push<float>(1);//TextureIndex
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
    }

    void VertexIndex::OnRender()
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

    void VertexIndex::OnImGuiRender()
    {
        ImGui::SliderFloat3("positionA", &positionA.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("positionB", &positionB.x, -900.0f, 900.0f);
    }
}
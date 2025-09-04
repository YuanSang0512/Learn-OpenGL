#include "LightTest.h"

#include "Renderer.h"
#include "InputProcess.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include <array>

struct vec3 {
    float x, y, z;
};

struct vec4 {
    float x, y, z, w;
};

struct vertex {
    vec3 position;
    vec4 color;
};

static vertex* CreateCube(vertex* target, glm::vec3 position, glm::vec4 color, float size)
{
	float m_x = position.x;
	float m_y = position.y;
	float m_z = position.z;
	float c_R = color.r;
	float c_M = color.g;
	float c_B = color.b;
    float r = size / 2.0f;
    // Front face
    // 0
    target->position = { m_x + r, m_y + r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 1
    target->position = { m_x - r, m_y + r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 2
    target->position = { m_x - r, m_y - r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 3
    target->position = { m_x + r, m_y - r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // Back face
    // 4
    target->position = { m_x + r, m_y + r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 5
    target->position = { m_x - r, m_y + r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 6
    target->position = { m_x - r, m_y - r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 7
    target->position = { m_x + r, m_y - r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // Right face
    // 8
    target->position = { m_x + r, m_y + r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 9
    target->position = { m_x + r, m_y + r, m_z + r }; 
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 10
    target->position = { m_x + r, m_y - r, m_z + r };    
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 11
    target->position = { m_x + r, m_y - r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // Left face
    // 12
    target->position = { m_x - r, m_y + r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 13
    target->position = { m_x - r, m_y + r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 14
    target->position = { m_x - r, m_y - r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 15
    target->position = { m_x - r, m_y - r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // Top face
    // 16
    target->position = { m_x + r, m_y + r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 17
    target->position = { m_x - r, m_y + r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 18
    target->position = { m_x - r, m_y + r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 19
    target->position = { m_x + r, m_y + r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // Bottom face
    // 20
    target->position = { m_x + r, m_y - r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 21
    target->position = { m_x - r, m_y - r, m_z - r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 22
    target->position = { m_x - r, m_y - r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    // 23
    target->position = { m_x + r, m_y - r, m_z + r };
    target->color = { c_R, c_M, c_B, 1.0f };
    target++;

    return target;
}

static unsigned int* CreateCubeIndices(unsigned int* indices, const int MaxIndexCount)
{
    int offset = 0;
    indices = new unsigned int[MaxIndexCount];
    for (int i = 0; i < MaxIndexCount; i += 36)
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

namespace test
{
    const int MaxCubeCount = 2;
    const int MaxVertexCount = MaxCubeCount * 24;
    const int MaxIndexCount = MaxCubeCount * 36;

    LightTest::LightTest()
		: cameraPos(0, 0, 3), rot_x(0), rot_y(0), rot_z(0), 
		positionA(0, 0, 0), positionB(300, 300, 0), m_lightColor(1, 1, 1, 1), m_toyColor(1, 0.5, 0, 1)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));//启用混合

        unsigned int* indices = NULL;
        indices = CreateCubeIndices(indices, MaxIndexCount);

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(vertex), true);//坐标缓冲对象
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

        m_Shader = std::make_unique<Shader>("res/shaders/LightTest.shader");
        m_Shader->Bind();
    }

    LightTest::~LightTest()
    {

    }

    void LightTest::OnUpdate(float deltaTime)
    {
        std::array<vertex, MaxVertexCount> vertices;
        vertex* buffer = vertices.data();

		glm::vec4 lightColor = glm::vec4(m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w);
		glm::vec4 toyColor = glm::vec4(m_toyColor.x, m_toyColor.y, m_toyColor.z, m_toyColor.w);
        toyColor *= lightColor;
        buffer = CreateCube(buffer, positionA, lightColor, 200.0f);
        buffer = CreateCube(buffer, positionB, toyColor, 300.0f);

        m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());

        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        layout.Push<float>(4);//Color
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
    }

    void LightTest::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer render;

        m_proj = glm::ortho(-900.0f, 900.0f, -900.0f, 900.0f, -900.0f, 900.0f);

        //旋转摄像机
        //m_view = glm::rotate(glm::mat4(1.0f), glm::radians(rot_x), glm::vec3(1, 0, 0));
        //m_view = glm::rotate(m_view, glm::radians(rot_y), glm::vec3(0, 1, 0));
        //m_view = glm::rotate(m_view, glm::radians(rot_z), glm::vec3(0, 0, 1));
        //float radius = 10.0f;
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;

        cameraPos = ProcessInput(cameraPos, 8.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 m_model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        m_model = glm::translate(m_model, glm::vec3(0, 0, 0));
        glm::mat4 mvp = m_proj * m_view * m_model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

    void LightTest::OnImGuiRender()
    {
        ImGui::SliderFloat3("positionA", &positionA.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("positionB", &positionB.x, -900.0f, 900.0f);
		ImGui::SliderFloat4("m_lightColor", &m_lightColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat4("m_toyColor", &m_toyColor.x, 0.0f, 1.0f);
        //ImGui::SliderFloat("rot_x", &rot_x, -180.0f, 180.0f);
        //ImGui::SliderFloat("rot_y", &rot_y, -180.0f, 180.0f);
        //ImGui::SliderFloat("rot_z", &rot_z, -180.0f, 180.0f);
    }
}
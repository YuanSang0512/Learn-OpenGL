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
};

static vertex* CreateCube(vertex* target, float size)
{
    float r = size / 2.0f;
    // Front face
    // 0
    target->position = {  r,  r, -r };
    target++;

    // 1
    target->position = { -r,  r, -r };
    target++;

    // 2
    target->position = { -r, -r, -r };
    target++;

    // 3
    target->position = {  r, -r, -r };
    target++;

    // Back face
    // 4
    target->position = {  r,  r,  r };
    target++;

    // 5
    target->position = { -r,  r,  r };
    target++;

    // 6
    target->position = { -r, -r,  r };
    target++;

    // 7
    target->position = {  r, -r,  r };
    target++;

    // Right face
    // 8
    target->position = {  r,  r, -r };
    target++;

    // 9
    target->position = {  r,  r,  r }; 
    target++;

    // 10
    target->position = {  r, -r,  r };    
    target++;

    // 11
    target->position = {  r, -r, -r };
    target++;

    // Left face
    // 12
    target->position = { -r,  r, -r };
    target++;

    // 13
    target->position = { -r,  r,  r };
    target++;

    // 14
    target->position = { -r, -r,  r };
    target++;

    // 15
    target->position = { -r, -r, -r };
    target++;

    // Top face
    // 16
    target->position = {  r,  r, -r };
    target++;

    // 17
    target->position = { -r,  r, -r };
    target++;

    // 18
    target->position = { -r,  r,  r };
    target++;

    // 19
    target->position = {  r,  r,  r };
    target++;

    // Bottom face
    // 20
    target->position = {  r, -r, -r };
    target++;

    // 21
    target->position = { -r, -r, -r };
    target++;

    // 22
    target->position = { -r, -r,  r };
    target++;

    // 23
    target->position = {  r, -r,  r };
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
    const int MaxCubeCount = 1;
    const int MaxVertexCount = MaxCubeCount * 24;
    const int MaxIndexCount = MaxCubeCount * 36;

    LightTest::LightTest()
		: cameraPos(0, 0, 3), rot_x(0), rot_y(0), rot_z(0), 
        lightPos(0, 0, 0), objectPos(300, 300, 0), m_lightColor(1, 1, 1), m_toyColor(1, 0.5, 0)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));//启用混合

        unsigned int* indices = NULL;
        indices = CreateCubeIndices(indices, MaxIndexCount);

        m_VBO = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(vertex), true);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

        m_LightVAO = std::make_unique<VertexArray>();
        m_LightShader = std::make_unique<Shader>("res/shaders/LightCube.shader");
        
        m_ObjectVAO = std::make_unique<VertexArray>();
        m_ObjectShader = std::make_unique<Shader>("res/shaders/Object.shader");
        
        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        m_LightVAO->AddBuffer(*m_VBO, layout);
        m_ObjectVAO->AddBuffer(*m_VBO, layout);
    }

    LightTest::~LightTest()
    {

    }

    void LightTest::OnUpdate(float deltaTime)
    {
        std::array<vertex, MaxVertexCount> vertices;
        vertex* buffer = vertices.data();

        buffer = CreateCube(buffer, 200.0f);
        m_VBO->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
    }

    void LightTest::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        cameraPos = ProcessInput(cameraPos, 8.0f);

        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)800 / 800; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        m_proj = glm::perspective(fov, aspect, nearPlane, farPlane);

        //m_proj = glm::ortho(-1500.0f, 1500.0f, -1500.0f, 1500.0f, -900.0f, 900.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        m_view = glm::rotate(m_view, glm::radians(rot_y), glm::vec3(0, 1, 0));
		m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        Renderer render;
        {
            m_model = glm::translate(glm::mat4(1.0f), lightPos);
            m_mvp = m_proj * m_view * m_model;

            m_LightShader->Bind();
            m_LightShader->SetUniformMat4f("u_MVP", m_mvp);
            m_LightShader->SetUniformVec4f("u_Color", glm::vec4(m_lightColor, 1.0f));
            render.Draw(*m_LightVAO, *m_IndexBuffer, *m_LightShader);
        }
        {
			m_model = glm::translate(glm::mat4(1.0f), objectPos);
            m_mvp = m_proj * m_view * m_model;
            float ambientStrength = 0.2f;
            glm::vec3 ambientColor = ambientStrength * m_lightColor;
            f_toyColor = ambientColor * m_toyColor;

            m_ObjectShader->Bind();
            m_ObjectShader->SetUniformMat4f("u_MVP", m_mvp);
            m_LightShader->SetUniformVec4f("u_Color", glm::vec4(f_toyColor, 1.0f));
            render.Draw(*m_ObjectVAO, *m_IndexBuffer, *m_ObjectShader);
        }
    }

    void LightTest::OnImGuiRender()
    {
        ImGui::SliderFloat3("lightPos", &lightPos.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("objectPos", &objectPos.x, -900.0f, 900.0f);
		ImGui::SliderFloat3("m_lightColor", &m_lightColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("m_toyColor", &m_toyColor.x, 0.0f, 1.0f);
        //ImGui::SliderFloat("rot_x", &rot_x, -180.0f, 180.0f);
        ImGui::SliderFloat("rot_y", &rot_y, -180.0f, 180.0f);
        //ImGui::SliderFloat("rot_z", &rot_z, -180.0f, 180.0f);
    }
}
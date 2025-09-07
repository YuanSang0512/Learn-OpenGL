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
	vec3 normal;
};
float radius = 300.0f;

static vertex* CreateCube(vertex* target, float size)
{
    float r = size / 2.0f;

    // Front face (+Z)
    vec3 frontNormal = { 0.0f, 0.0f, -1.0f };
    target->position = { r,  r, -r }; target->normal = frontNormal; target++;
    target->position = { -r,  r, -r }; target->normal = frontNormal; target++;
    target->position = { -r, -r, -r }; target->normal = frontNormal; target++;
    target->position = { r, -r, -r }; target->normal = frontNormal; target++;

    // Back face (-Z)
    vec3 backNormal = { 0.0f, 0.0f, 1.0f };
    target->position = { r,  r,  r }; target->normal = backNormal; target++;
    target->position = { -r,  r,  r }; target->normal = backNormal; target++;
    target->position = { -r, -r,  r }; target->normal = backNormal; target++;
    target->position = { r, -r,  r }; target->normal = backNormal; target++;

    // Right face (+X)
    vec3 rightNormal = { 1.0f, 0.0f, 0.0f };
    target->position = { r,  r, -r }; target->normal = rightNormal; target++;
    target->position = { r,  r,  r }; target->normal = rightNormal; target++;
    target->position = { r, -r,  r }; target->normal = rightNormal; target++;
    target->position = { r, -r, -r }; target->normal = rightNormal; target++;

    // Left face (-X)
    vec3 leftNormal = { -1.0f, 0.0f, 0.0f };
    target->position = { -r,  r, -r }; target->normal = leftNormal; target++;
    target->position = { -r,  r,  r }; target->normal = leftNormal; target++;
    target->position = { -r, -r,  r }; target->normal = leftNormal; target++;
    target->position = { -r, -r, -r }; target->normal = leftNormal; target++;

    // Top face (+Y)
    vec3 topNormal = { 0.0f, 1.0f, 0.0f };
    target->position = { r,  r, -r }; target->normal = topNormal; target++;
    target->position = { -r,  r, -r }; target->normal = topNormal; target++;
    target->position = { -r,  r,  r }; target->normal = topNormal; target++;
    target->position = { r,  r,  r }; target->normal = topNormal; target++;

    // Bottom face (-Y)
    vec3 bottomNormal = { 0.0f, -1.0f, 0.0f };
    target->position = { r, -r, -r }; target->normal = bottomNormal; target++;
    target->position = { -r, -r, -r }; target->normal = bottomNormal; target++;
    target->position = { -r, -r,  r }; target->normal = bottomNormal; target++;
    target->position = { r, -r,  r }; target->normal = bottomNormal; target++;

    return target;
}


static std::vector<unsigned int> CreateCubeIndices(std::vector<unsigned int> indices)
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

namespace test
{
    const int MaxCubeCount = 1;
    const int MaxVertexCount = MaxCubeCount * 24;
    const int MaxIndexCount = MaxCubeCount * 36;

    LightTest::LightTest()
		:lightPos(0, 0, 300), objectPos(0, 0, 0), m_lightColor(1, 1, 1), m_toyColor(1, 0.5, 0),
		ambientColor(0.19225, 0.19225, 0.19225), diffuseColor(0.50754, 0.50754, 0.50754), specularColor(0.508273, 0.508273, 0.508273),
		ambientLight(0.2, 0.2, 0.2), diffuseLight(0.5, 0.5, 0.5), specularLight(1.0, 1.0, 1.0),
        shininess(0.4*128)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

		std::vector<unsigned int> indices(MaxIndexCount);
        indices = CreateCubeIndices(indices);

        m_VBO = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(vertex), true);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), MaxIndexCount);

        m_LightVAO = std::make_unique<VertexArray>();
        m_LightShader = std::make_unique<Shader>("res/shaders/LightCube.shader");
        
        m_ObjectVAO = std::make_unique<VertexArray>();
        m_ObjectShader = std::make_unique<Shader>("res/shaders/Object.shader");
        
        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
		layout.Push<float>(3);//Normal
        m_LightVAO->AddBuffer(*m_VBO, layout);
        m_ObjectVAO->AddBuffer(*m_VBO, layout);
    }

    LightTest::~LightTest()
    {

    }

    void LightTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();

        std::array<vertex, MaxVertexCount> vertices;
        vertex* buffer = vertices.data();

        buffer = CreateCube(buffer, 300.0f);
        m_VBO->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
    }

    void LightTest::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)800 / 800; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 1500.0f;
		m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();

        Renderer render;
        {
            float speed = 1.0f; // 旋转速度倍数
            float theta = glfwGetTime() * speed;

            lightPos.x = cos(theta) * radius;
            lightPos.z = sin(theta) * radius;
			//std::cout << lightPos.x << "," << lightPos.y << "," << lightPos.z << std::endl;
			m_model = glm::translate(glm::mat4(1.0f), lightPos);
            m_model = glm::scale(m_model, glm::vec3(0.05f));

            m_LightShader->Bind();
            m_LightShader->SetUniformMat4f("u_Projection", m_proj);
            m_LightShader->SetUniformMat4f("u_View", m_view);
            m_LightShader->SetUniformMat4f("u_Model", m_model);
            m_LightShader->SetUniformVec3f("u_Color", m_lightColor);
            render.Draw(*m_LightVAO, *m_IndexBuffer, *m_LightShader);
        }
        {
			m_model = glm::translate(glm::mat4(1.0f), objectPos);
			m_model = glm::rotate(m_model, glm::radians(rx), glm::vec3(1, 0, 0));
			rx += 0.5f;

            m_ObjectShader->Bind();
            m_ObjectShader->SetUniformMat4f("u_Projection", m_proj);
            m_ObjectShader->SetUniformMat4f("u_View", m_view);
            m_ObjectShader->SetUniformMat4f("u_Model", m_model);

            m_ObjectShader->SetUniformVec3f("u_material.ambient", ambientColor);
            m_ObjectShader->SetUniformVec3f("u_material.diffuse", diffuseColor);
            m_ObjectShader->SetUniformVec3f("u_material.specular", specularColor);
            m_ObjectShader->SetUniform1f("u_material.shininess", shininess);

            m_ObjectShader->SetUniformVec3f("u_Light.lightPos", lightPos);
            m_ObjectShader->SetUniformVec3f("u_Light.ambient", ambientLight);
            m_ObjectShader->SetUniformVec3f("u_Light.diffuse", diffuseLight);
            m_ObjectShader->SetUniformVec3f("u_Light.specular", specularLight);

            m_ObjectShader->SetUniformVec3f("u_CamPos", cam.GetPosition());            
            render.Draw(*m_ObjectVAO, *m_IndexBuffer, *m_ObjectShader);
        }

    }

    void LightTest::OnImGuiRender()
    {
        ImGui::Text("Transformations");
        ImGui::SliderFloat("radius", &radius, 0.0f, 500.0f);
        ImGui::SliderFloat3("lightPos", &lightPos.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("objectPos", &objectPos.x, -900.0f, 900.0f);
        ImGui::Separator();

		ImGui::Text("Object Material");
        ImGui::SliderFloat3("ambientColor", &ambientColor.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("diffuseColor", &diffuseColor.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("specularColor", &specularColor.x, 0.0f, 1.0f);
        ImGui::SliderFloat("shininess", &shininess, 0.0f, 256.0f);

		ImGui::Text("Light Strength");
        ImGui::SliderFloat3("ambientLight", &ambientLight.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("diffuseLight", &diffuseLight.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("specularLight", &specularLight.x, 0.0f, 1.0f);
    }
}
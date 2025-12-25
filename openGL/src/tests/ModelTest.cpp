#include <array>

#include "ModelTest.h"
#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    ModelTest::ModelTest()
        :lightPos(0, 0, 300), objectPos(0, 0, 0), m_lightColor(1, 1, 1), shininess(32),
        ambientLight(0.4, 0.4, 0.4), diffuseLight(0.4, 0.4, 0.4), specularLight(0.5, 0.5, 0.5)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

		//光源
        BasicModels basicModels;
		std::vector<unsigned int> indices(36);
		indices = basicModels.CreateCubeIndices(indices);

        std::array<vertex, 24> vertices;
        vertex* buffer = vertices.data();
        buffer = basicModels.CreateCubeVertexs(buffer, 300.0f);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), 36);
		m_LightVBO = std::make_unique<VertexBuffer>(vertices.data(), 24 * sizeof(vertex), false);
		m_LightVAO = std::make_unique<VertexArray>();
		m_LightShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/LightCube.shader");
		VertexBufferLayout layout;
		layout.Push<float>(3);//Position
		layout.Push<float>(3);//Normal
		layout.Push<float>(2);//TexCoord
		m_LightVAO->AddBuffer(*m_LightVBO, layout);

        //物体
        m_Shader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/ModelTest.shader");
    }

    ModelTest::~ModelTest()
    {

    }

    void ModelTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();

    }

    void ModelTest::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)800 / 800; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 1500.0f;
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();

        {
            m_model = glm::translate(glm::mat4(1.0f), objectPos);
            m_model = glm::rotate(m_model, glm::radians(ry), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(50.0f));
            ry += 0.5f;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Projection", m_proj);
            m_Shader->SetUniformMat4f("u_View", m_view);
            m_Shader->SetUniformMat4f("u_Model", m_model);

            m_Shader->SetUniform1f("u_Material.shininess", shininess);

            m_Shader->SetUniformVec3f("u_Light.lightPos", lightPos);
            m_Shader->SetUniformVec3f("u_Light.ambient", ambientLight);
            m_Shader->SetUniformVec3f("u_Light.diffuse", diffuseLight);
            m_Shader->SetUniformVec3f("u_Light.specular", specularLight);

            m_Shader->SetUniformVec3f("u_CamPos", cam.GetPosition());

            backpack.Draw(*m_Shader);
        }

        Renderer render;
        {
            float speed = 1.0f; // 旋转速度倍数
            float theta = glfwGetTime() * speed;

            lightPos.x = cos(theta) * radius;
            lightPos.z = sin(theta) * radius;

			m_model = glm::translate(glm::mat4(1.0f), lightPos);
			m_model = glm::scale(m_model, glm::vec3(0.05f));

			m_LightShader->Bind();
			m_LightShader->SetUniformMat4f("u_Projection", m_proj);
			m_LightShader->SetUniformMat4f("u_View", m_view);
			m_LightShader->SetUniformMat4f("u_Model", m_model);

            m_LightShader->SetUniformVec3f("u_Color", m_lightColor);

			render.Draw(*m_LightVAO, *m_IndexBuffer, *m_LightShader);
        }

    }

    void ModelTest::OnImGuiRender()
    {
        ImGui::Text("Transformations");
        ImGui::SliderFloat("radius", &radius, 0.0f, 500.0f);
        ImGui::SliderFloat3("lightPos", &lightPos.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("objectPos", &objectPos.x, -900.0f, 900.0f);
        ImGui::Separator();

        ImGui::Text("Object Material");
        ImGui::SliderFloat("shininess", &shininess, 1.0f, 256.0f);

        ImGui::Text("Light Strength");
        ImGui::SliderFloat3("ambientLight", &ambientLight.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("diffuseLight", &diffuseLight.x, 0.0f, 1.0f);
        ImGui::SliderFloat3("specularLight", &specularLight.x, 0.0f, 1.0f);
    }


}
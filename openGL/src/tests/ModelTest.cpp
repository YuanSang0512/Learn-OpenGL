#include "ModelTest.h"

#include "InputProcess.h"
#include "imgui/imgui.h"

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>

namespace test
{
    ModelTest::ModelTest()
        :lightPos(0, 0, 300), objectPos(0, 0, 0), m_lightColor(1, 1, 1), m_toyColor(1, 0.5, 0),
        ambientColor(0.19225, 0.19225, 0.19225), specularColor(0.508273, 0.508273, 0.508273),
        ambientLight(0.4, 0.4, 0.4), diffuseLight(0.5, 0.5, 0.5), specularLight(1.0, 1.0, 1.0),
        shininess(0.4 * 128)
    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

        m_Shader = std::make_unique<Shader>("res/shaders/ModelTest.shader");
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

            backpack.Draw(*m_Shader);
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
#include <array>

#include "StencilTest.h"

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    StencilTest::StencilTest()
        :lightPos(0, 0, 300), objectPos(0, 0, 0), m_lightColor(1, 1, 0), shininess(32),
        ambientLight(0.4, 0.4, 0.4), diffuseLight(0.4, 0.4, 0.4), specularLight(0.5, 0.5, 0.5)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

        //光源
        BasicModel::BasicModels basicModels;

        std::array<BasicModel::Vertex, 24> vertices = basicModels.CreateCubeVertexs(300.0f);
        std::vector<unsigned int> indices = basicModels.CreateCubeIndices();

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices.data(), 36);

        //光源
        m_LightVBO = std::make_unique<VertexBuffer>(vertices.data(), 24 * sizeof(BasicModel::Vertex), false);
        m_LightVAO = std::make_unique<VertexArray>();
        m_LightShader = std::make_unique<Shader>("res/shaders/LightCube.shader");

        //物体
        m_ObjectVBO = std::make_unique<VertexBuffer>(vertices.data(), 24 * sizeof(BasicModel::Vertex), false);
        m_ObjectVAO = std::make_unique<VertexArray>();
        m_ObjectShader = std::make_unique<Shader>("res/shaders/Cube.shader");

		//物体轮廓
        m_OutlineShader = std::make_unique<Shader>("res/shaders/Cube.shader");

        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        layout.Push<float>(3);//Normal
        layout.Push<float>(2);//TexCoord
        m_LightVAO->AddBuffer(*m_LightVBO, layout);
		m_ObjectVAO->AddBuffer(*m_ObjectVBO, layout);
    }

    StencilTest::~StencilTest()
    {

    }

    void StencilTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
    }

    void StencilTest::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)800 / 800; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 1500.0f;
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();

        Renderer render;
        //物体
        {
            GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
            GLCall(glStencilMask(0xFF)); // 启用模板缓冲写入
            m_model = glm::translate(glm::mat4(1.0f), objectPos);
            m_model = glm::rotate(m_model, glm::radians(ry), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.0f));

            m_ObjectShader->Bind();
            m_ObjectShader->SetUniformMat4f("u_Projection", m_proj);
            m_ObjectShader->SetUniformMat4f("u_View", m_view);
            m_ObjectShader->SetUniformMat4f("u_Model", m_model);
            m_ObjectShader->SetUniformVec3f("u_Color", glm::vec3(1.0, 1.0, 1.0));
            render.Draw(*m_ObjectVAO, *m_IndexBuffer, *m_ObjectShader);
        }
        //遮挡物
        {
            GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
            GLCall(glStencilMask(0x00)); // 禁用模板缓冲写入
            m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 500));
            m_model = glm::rotate(m_model, glm::radians(ry), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.0f));

            m_ObjectShader->Bind();
            m_ObjectShader->SetUniformMat4f("u_Projection", m_proj);
            m_ObjectShader->SetUniformMat4f("u_View", m_view);
            m_ObjectShader->SetUniformMat4f("u_Model", m_model);
            m_ObjectShader->SetUniformVec3f("u_Color", glm::vec3(0.0, 1.0, 1.0));
            render.Draw(*m_ObjectVAO, *m_IndexBuffer, *m_ObjectShader);
        }
        //轮廓
        {
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
			glDepthMask(GL_FALSE);
            glDepthFunc(GL_ALWAYS);
            m_model = glm::translate(glm::mat4(1.0f), objectPos);
            m_model = glm::rotate(m_model, glm::radians(ry), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.05f));
            ry += 0.5f;
			m_OutlineShader->Bind();
			m_OutlineShader->SetUniformMat4f("u_Projection", m_proj);
			m_OutlineShader->SetUniformMat4f("u_View", m_view);
			m_OutlineShader->SetUniformMat4f("u_Model", m_model);
            m_OutlineShader->SetUniformVec3f("u_Color", glm::vec3(1.0, 0.0, 0.0));
			render.Draw(*m_ObjectVAO, *m_IndexBuffer, *m_OutlineShader);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
        }
        //光源
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

    void StencilTest::OnImGuiRender()
    {
        ImGui::Text("Transformations");
        ImGui::SliderFloat("radius", &radius, 0.0f, 500.0f);
        ImGui::SliderFloat3("lightPos", &lightPos.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("objectPos", &objectPos.x, -900.0f, 900.0f);
    }
}
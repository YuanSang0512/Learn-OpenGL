#include "BlendingTest.h"

#include <array>

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    BlendingTest::BlendingTest()
        :objectPos(0, 0, 0), m_CubeColor(1, 1, 0), shininess(32),
        ambientLight(0.4, 0.4, 0.4), diffuseLight(0.4, 0.4, 0.4), specularLight(0.5, 0.5, 0.5)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

        BasicModels basicModels;

        std::array<vertex, 24> vertices_object;
        vertex* buffer = vertices_object.data();
        buffer = basicModels.CreateCubeVertexs(buffer, 300.0f);
        std::vector<unsigned int> indices_object(36);
        indices_object = basicModels.CreateCubeIndices(indices_object);

		std::array<vertex, 4> vertices_grass;
		vertex* buffer_grass = vertices_grass.data();
		buffer_grass = basicModels.CreatePlaneVertexs(buffer_grass, { 300.0f,300.0f }, { 0.0f,0.0f,1.0f });
		std::vector<unsigned int> indices_grass(6);
		indices_grass = basicModels.CreatePlaneIndices(indices_grass);

        //物体
        m_ObjectVAO = std::make_unique<VertexArray>();
        m_ObjectVBO = std::make_unique<VertexBuffer>(vertices_object.data(), 24 * sizeof(vertex), false);
        m_ObjectEBO = std::make_unique<IndexBuffer>(indices_object.data(), 36);
        m_ObjectShader = std::make_unique<Shader>("res/shaders/Cube.shader");

		//草地纹理
		m_GrassVAO = std::make_unique<VertexArray>();
		m_GrassVBO = std::make_unique<VertexBuffer>(vertices_grass.data(), 4 * sizeof(vertex), false);
        m_GrassEBO = std::make_unique<IndexBuffer>(indices_grass.data(), 6);
        m_GrassTexture = std::make_unique<Texture>("res/textures/grass.png", TextureType::TEXTURE_2D);
		m_GrassShader = std::make_unique<Shader>("res/shaders/BlendingTest.shader");

		//窗户纹理
		m_WindowTexture = std::make_unique<Texture>("res/textures/blending_transparent_window.png", TextureType::TEXTURE_2D);


        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        layout.Push<float>(3);//Normal
        layout.Push<float>(2);//TexCoord
        m_ObjectVAO->AddBuffer(*m_ObjectVBO, layout);
		m_GrassVAO->AddBuffer(*m_GrassVBO, layout);
    }

    BlendingTest::~BlendingTest()
    {

    }

    void BlendingTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
    }

    void BlendingTest::OnRender()
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
        //物体
        {
            m_model = glm::translate(glm::mat4(1.0f), objectPos);
            m_model = glm::rotate(m_model, glm::radians(0.0f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.0f));

            m_ObjectShader->Bind();
            m_ObjectShader->SetUniformMat4f("u_Projection", m_proj);
            m_ObjectShader->SetUniformMat4f("u_View", m_view);
            m_ObjectShader->SetUniformMat4f("u_Model", m_model);
            m_ObjectShader->SetUniformVec3f("u_Color", m_CubeColor);
            //render.Draw(*m_ObjectVAO, *m_ObjectEBO, *m_ObjectShader);
        }
		//草/窗户
        {
            m_model = glm::translate(glm::mat4(1.0f), objectPos + glm::vec3(0, 0, 200));
            m_model = glm::rotate(m_model, glm::radians(0.0f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.0f));

			m_GrassShader->Bind();
			m_GrassTexture->Bind(0);
            m_WindowTexture->Bind(1);
			m_GrassShader->SetUniform1i("u_Texture", 0);
			m_GrassShader->SetUniformMat4f("u_Projection", m_proj);
			m_GrassShader->SetUniformMat4f("u_View", m_view);
			m_GrassShader->SetUniformMat4f("u_Model", m_model);
			render.Draw(*m_GrassVAO, *m_GrassEBO, *m_GrassShader);
        }
        //草/窗户
        {
            m_model = glm::translate(glm::mat4(1.0f), objectPos + glm::vec3(0, 0, 250));
            m_model = glm::rotate(m_model, glm::radians(0.0f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.0f));

            m_GrassShader->Bind();
            m_GrassTexture->Bind(0);
            m_WindowTexture->Bind(1);
            m_GrassShader->SetUniform1i("u_Texture", 1);
            m_GrassShader->SetUniformMat4f("u_Projection", m_proj);
            m_GrassShader->SetUniformMat4f("u_View", m_view);
            m_GrassShader->SetUniformMat4f("u_Model", m_model);
            render.Draw(*m_GrassVAO, *m_GrassEBO, *m_GrassShader);
        }

    }

    void BlendingTest::OnImGuiRender()
    {
        ImGui::Text("Transformations");
        ImGui::SliderFloat("radius", &radius, 0.0f, 500.0f);
        ImGui::SliderFloat3("objectPos", &objectPos.x, -900.0f, 900.0f);
		ImGui::SliderFloat3("CubeColor", &m_CubeColor.x, 0.0f, 1.0f);
    }
}
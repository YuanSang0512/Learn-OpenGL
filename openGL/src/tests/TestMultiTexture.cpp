#include "TestMultiTexture.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    TestMultiTexture::TestMultiTexture() : translation(200, 200, 0)
	{
        //          position                color             texCoord  texIndex
        //    --------------------- | --------------------- | --------- | ---
        float positions[] = {
                0.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
              200.0f,    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
              200.0f,  200.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                0.0f,  200.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,

             -200.0f, -200.0f, 0.0f, 0.7f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                0.0f, -200.0f, 0.0f, 0.0f, 0.4f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                0.0f,    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             -200.0f,    0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f
        };
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
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 80 * sizeof(float));//坐标缓冲对象
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));

        VertexBufferLayout layout;
        layout.Push<float>(3);//Position
        layout.Push<float>(4);//Color
        layout.Push<float>(2);//TexCoord
        layout.Push<float>(1);//TextureIndex

        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_proj = glm::ortho(-900.0f, 900.0f, -900.0f, 900.0f, -900.0f, 900.0f);//正交投影 渲染的边界（上下左右前后）
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader = std::make_unique<Shader>("res/shaders/MultiTexture.shader");
        m_Shader->Bind();

        m_Texture1 = std::make_unique<Texture>("res/textures/earth.png", TextureType::TEXTURE_2D);
        m_Texture2 = std::make_unique<Texture>("res/textures/maodie.jpg", TextureType::TEXTURE_2D);
	}

    TestMultiTexture::~TestMultiTexture()
	{

	}

	void TestMultiTexture::OnUpdate(float deltaTime)
	{

	}

	void TestMultiTexture::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer render;
        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

        int sampler[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, sampler);


        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  model = glm::translate(model, translation);
        glm::mat4 mvp = m_proj * m_view * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

	void TestMultiTexture::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 900.0f);
	}
}
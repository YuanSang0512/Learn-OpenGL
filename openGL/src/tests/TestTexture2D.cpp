#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D() : translation(200, 200, 0)
	{
        float positions[] = {
                0.0f,    0.0f, 0.0f, 0.0f, //0
              500.0f,    0.0f, 1.0f, 0.0f, //1
              500.0f,  500.0f, 1.0f, 1.0f, //2
                0.0f,  500.0f, 0.0f, 1.0f//3
        };
        //用于指定坐标读取顺序
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));//启用混合

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));//坐标缓冲对象
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        VertexBufferLayout layout;
        layout.Push<float>(2);//Position
        layout.Push<float>(2);//Texture

        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_proj = glm::ortho(-900.0f, 900.0f, -900.0f, 900.0f, -900.0f, 900.0f);//正交投影 渲染的边界（上下左右前后）
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/earth.png", TextureType::TEXTURE_2D);
        m_Shader->SetUniform1i("u_Texture", 0);//绑定纹理到插槽0
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer render;
        m_Texture->Bind();

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  model = glm::translate(model, translation);
        glm::mat4 mvp = m_proj * m_view * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 900.0f);
	}
}
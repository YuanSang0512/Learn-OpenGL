#include "ColorfulCube.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    ColorfulCube::ColorfulCube()
	{
        float positions[] = {
                0.0f,    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
              200.0f,    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
              200.0f,  200.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                0.0f,  200.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f,

             -200.0f, -200.0f, 0.0f, 0.7f, 0.0f, 1.0f, 1.0f,
                0.0f, -200.0f, 0.0f, 0.0f, 0.4f, 0.0f, 1.0f,
                0.0f,    0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
             -200.0f,    0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 1.0f
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
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions) * sizeof(float));//坐标缓冲对象
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(4);

        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_proj = glm::ortho(-900.0f, 900.0f, -900.0f, 900.0f, -900.0f, 900.0f);//正交投影 渲染的边界（上下左右前后）
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_Shader = std::make_unique<Shader>("res/shaders/ColorfulCube.shader");
        m_Shader->Bind();
	}

    ColorfulCube::~ColorfulCube()
	{

	}

	void ColorfulCube::OnUpdate(float deltaTime)
	{

	}

	void ColorfulCube::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer render;

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  model = glm::translate(model, translation);
        glm::mat4 mvp = m_proj * m_view * model;
        m_Shader->SetUniformMat4f("u_MVP", mvp);

        render.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }

	void ColorfulCube::OnImGuiRender()
	{
	}
}
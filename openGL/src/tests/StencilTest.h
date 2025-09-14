#pragma once

#include <GL/glew.h> 
#include "GLFW/glfw3.h"

#include "Test.h"
#include "Camera.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Model.h"

#include <memory>

namespace test
{
	class StencilTest : public Test
	{
	public:
		StencilTest();
		~StencilTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Camera cam;
		//Model backpack = Model("res/models/backpack/backpack.obj");
	private:
		//��Դ
		std::unique_ptr<VertexBuffer> m_LightVBO;
		std::unique_ptr<VertexArray> m_LightVAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_LightShader;

		//����
		std::unique_ptr<VertexBuffer> m_ObjectVBO;
		std::unique_ptr<VertexArray> m_ObjectVAO;
		std::unique_ptr<Shader> m_ObjectShader;

		//��������
		std::unique_ptr<Shader> m_OutlineShader;

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_lightColor;
		glm::vec3 lightPos, objectPos, cameraPos;
		glm::vec3 ambientLight, diffuseLight, specularLight;//���ƹ���Ӱ��ǿ��
		float shininess, ry = 0, radius = 300.0f;
	};
}
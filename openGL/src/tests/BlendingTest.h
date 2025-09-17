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
	class BlendingTest : public Test
	{
	public:
		BlendingTest();
		~BlendingTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Camera cam;
	private:
		//物体
		std::unique_ptr<VertexBuffer> m_ObjectVBO;
		std::unique_ptr<VertexArray> m_ObjectVAO;
		std::unique_ptr<Shader> m_ObjectShader;
		std::unique_ptr<IndexBuffer> m_ObjectEBO;

		//草地
		std::unique_ptr<VertexBuffer> m_GrassVBO;
		std::unique_ptr<VertexArray> m_GrassVAO;
		std::unique_ptr<Shader> m_GrassShader;
		std::unique_ptr<IndexBuffer> m_GrassEBO;
		std::unique_ptr<Texture> m_GrassTexture;

		//窗户
		std::unique_ptr<Texture> m_WindowTexture;


		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_CubeColor;
		glm::vec3 objectPos, cameraPos;
		glm::vec3 ambientLight, diffuseLight, specularLight;//控制光照影响强度
		float shininess, ry = 0, radius = 300.0f;
	};
}
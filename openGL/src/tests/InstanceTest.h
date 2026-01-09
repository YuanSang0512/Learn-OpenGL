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
#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "Model.h"

#include <memory>
#include <vector>

namespace test
{
	class InstanceTest : public Test
	{
	public:
		InstanceTest();
		~InstanceTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void LoadModel();
		Camera cam;
	private:
#pragma region Camera
		float fov = glm::radians(80.0f); // 视野角度
		float aspect = (float)800 / 800; // 屏幕宽高比
		float nearPlane = 0.1f;
		float farPlane = 1500.0f;
#pragma endregion

		//天空盒
		std::unique_ptr<VertexArray> m_SkyBoxVAO;
		std::unique_ptr<VertexBuffer> m_SkyBoxVBO;
		std::unique_ptr<IndexBuffer> m_SkyBoxEBO;
		std::unique_ptr<Texture> m_SkyBoxTexture;
		std::unique_ptr<Shader> m_SkyBoxShader;

		//岩石
		std::unique_ptr<Shader> m_RockShader;
		float shininess = 32.0f, m_scale = 1.0f;
		glm::vec3 lightPos = glm::vec3(500.0f, 500.0f, 500.0f);
		glm::vec3 ambientLight = glm::vec3(0.4f, 0.4f, 0.4f);
		glm::vec3 diffuseLight = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 specularLight = glm::vec3(0.7f, 0.7f, 0.7f);

		//模型
		Model planet;
		Model rock;

		std::vector<glm::mat4> modelMatrices;

		std::unique_ptr<Shader> m_NormalShader;
		
		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 cameraPos;
	};
}
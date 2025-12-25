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
	class SkyBoxTest : public Test
	{
	public:
		SkyBoxTest();
		~SkyBoxTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Camera cam;
	private:
		#pragma region Camera
		float fov = glm::radians(80.0f); // 视野角度
		float aspect = (float)800 / 800; // 屏幕宽高比
		float nearPlane = 0.1f;
		float farPlane = 1500.0f;
		#pragma endregion

		//天空盒
		std::unique_ptr<VertexBuffer> m_SkyBoxVBO;
		std::unique_ptr<VertexArray> m_SkyBoxVAO;
		std::unique_ptr<Shader> m_SkyBoxShader;
		std::unique_ptr<IndexBuffer> m_SkyBoxEBO;
		std::unique_ptr<Texture> m_SkyBoxTexture;
		//立方体
		bool Cube = true;
		std::unique_ptr<VertexBuffer> m_ObjectVBO;
		std::unique_ptr<VertexArray> m_ObjectVAO;
		std::unique_ptr<Shader> m_ObjectShader;
		std::unique_ptr<IndexBuffer> m_ObjectEBO;
		std::unique_ptr<Texture> m_ObjectTexture;

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_CubeColor;
		glm::vec3 objectPos, cameraPos;
		glm::vec3 ambientLight, diffuseLight, specularLight;//控制光照影响强度
		float shininess, ry = 0, radius = 300.0f;
	};
}
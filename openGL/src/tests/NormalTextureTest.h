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
	class NormalTextureTest : public Test
	{
	public:
		NormalTextureTest();
		~NormalTextureTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		void LoadModel();
		void LoadTexture();
		void LoadShader();

		void CameraInit();
		Camera cam;
	private:
#pragma region Camera
		float fov = glm::radians(80.0f); // 视野角度
		float aspect = (float)800 / 800; // 屏幕宽高比
		float nearPlane = 0.1f;
		float farPlane = 1500.0f;
#pragma endregion
		//墙面
		std::unique_ptr<VertexArray> va_wall;
		std::unique_ptr<VertexBuffer> vb_wall;
		std::unique_ptr<IndexBuffer> ib_wall;

		std::unique_ptr<Texture> ambientTex_wall;
		std::unique_ptr<Texture> normalTex_wall;
		std::unique_ptr<Shader> shader_wall;

		//光源
		std::unique_ptr<VertexArray> va_light;
		std::unique_ptr<VertexBuffer> vb_light;
		std::unique_ptr<IndexBuffer> ib_light;

		std::unique_ptr<Shader> shader_light;


		glm::mat4 m_proj, m_view;
		glm::mat4 m_model_wall, m_model_light;
		float shininess = 32;
		glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 20.0f);
	};
}
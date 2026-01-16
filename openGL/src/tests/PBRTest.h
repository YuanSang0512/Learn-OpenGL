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
	class PBRTest : public Test
	{
	public:
		PBRTest();
		~PBRTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void LoadModel();
		void LoadShader();
		void LoadTexture();
		void CameraInit();
		Camera cam;

	private:
		void LightsInit();
		void PBRInit();


#pragma region Camera
		float fov = glm::radians(80.0f); // 视野角度
		float aspect = (float)1500 / 1500; // 屏幕宽高比
		float nearPlane = 0.1f;
		float farPlane = 1500.0f;
#pragma endregion

#pragma region PBR
		std::unique_ptr<VertexArray> VAO_PBR;
		std::unique_ptr<VertexBuffer> VBO_PBR;
		std::unique_ptr<IndexBuffer> IBO_PBR;
		std::unique_ptr<Shader> Shader_PBR;

		std::unique_ptr<Texture> albedoMap;
		std::unique_ptr<Texture> normalMap;
		std::unique_ptr<Texture> metallicMap;
		std::unique_ptr<Texture> roughnessMap;
		std::unique_ptr<Texture> aoMap;

		float metallic = 1.0f;
		float roughness = 1.0f;
#pragma endregion

#pragma region Light
		std::unique_ptr<VertexArray> VAO_Light;
		std::unique_ptr<VertexBuffer> VBO_Light;
		std::unique_ptr<VertexBuffer> VBO_Light_Instance;
		std::unique_ptr<IndexBuffer> IBO_Light;
		std::unique_ptr<Shader> Shader_Light;
#pragma endregion

		int lightCount = 4;//最大值受shader里的MAX_LIGHTS限制
		//std::vector<glm::mat4> instanceMatrices;
		glm::mat4 m_proj, m_view;

		float temp = 3.0f;
		glm::mat4 m_model_light0; glm::vec3 m_color_light0 = glm::vec3(1.0);
		glm::mat4 m_model_light1; glm::vec3 m_color_light1 = glm::vec3(1.0);
		glm::mat4 m_model_light2; glm::vec3 m_color_light2 = glm::vec3(1.0);
		glm::mat4 m_model_light3; glm::vec3 m_color_light3 = glm::vec3(1.0);
		glm::mat4 m_model_PBR;
		glm::vec3 cameraPos = glm::vec3(0, 0, 10);
		glm::vec3 PBR_Position = glm::vec3(0, 0, 0);
		float lightDistance = 10.0f;
	};
}
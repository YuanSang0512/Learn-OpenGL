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
	class TestName : public Test
	{
	public:
		TestName();
		~TestName();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void LoadModel();
		void CameraInit();
		Camera cam;
	private:
#pragma region Camera
		float fov = glm::radians(80.0f); // 视野角度
		float aspect = (float)800 / 800; // 屏幕宽高比
		float nearPlane = 0.1f;
		float farPlane = 1500.0f;
#pragma endregion

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 cameraPos;
	};
}
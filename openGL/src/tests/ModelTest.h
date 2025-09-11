#pragma once

#include <GL/glew.h>        // glew.h 必须在 glfw3.h 前
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
	class ModelTest : public Test
	{
	public:
		ModelTest();
		~ModelTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Camera cam;
		Model backpack = Model("res/models/LiuYing/liuying3.0.obj");
	private:
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_lightColor, m_toyColor, f_toyColor;
		glm::vec3 lightPos, objectPos, cameraPos;
		glm::vec3 ambientColor, specularColor;//控制物体材质,漫反射由texture控制
		glm::vec3 ambientLight, diffuseLight, specularLight;//控制光照强度
		float shininess, ry = 0, radius = 300.0f;
	};
}
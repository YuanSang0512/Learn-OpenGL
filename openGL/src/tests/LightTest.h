#pragma once
#include "Test.h"
#include "Renderer.h"
#include "Camera.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include <memory>

namespace test
{
	class LightTest : public Test
	{
	public:
		LightTest();
		~LightTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Camera cam;
	private:
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexArray> m_LightVAO, m_ObjectVAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_LightShader, m_ObjectShader;

		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_lightColor, m_toyColor, f_toyColor;
		glm::vec3 lightPos, objectPos, cameraPos;
		glm::vec3 ambientColor, diffuseColor, specularColor;//控制物体材质
		glm::vec3 ambientLight, diffuseLight, specularLight;//控制光照强度
		float shininess, rx = 0;
	};
}
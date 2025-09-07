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
	class LightingMaps : public Test
	{
	public:
		LightingMaps();
		~LightingMaps();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Camera cam;
	private:
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexArray> m_LightVAO, m_ObjectVAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_LightShader, m_ObjectShader;
		std::unique_ptr<Texture> m_Texture, m_Texture_Specular;


		glm::mat4 m_proj, m_view, m_model;
		glm::vec3 m_lightColor, m_toyColor, f_toyColor;
		glm::vec3 lightPos, objectPos, cameraPos;
		glm::vec3 ambientColor, specularColor;//控制物体材质,漫反射由texture控制
		glm::vec3 ambientLight, diffuseLight, specularLight;//控制光照强度
		float shininess, rx = 0, radius = 300.0f;
	};
}
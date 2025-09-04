#pragma once
#include "Test.h"
#include "Renderer.h"

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

	private:
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		glm::mat4 m_proj, m_view;
		glm::vec4 m_lightColor, m_toyColor;
		glm::vec3 positionA, positionB, cameraPos;
		float rot_x, rot_y, rot_z;
	};
}
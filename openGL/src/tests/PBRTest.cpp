#include "BasicTest.h"
#include "PBRTest.h"

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"

namespace test
{
    PBRTest::PBRTest()
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));
        CameraInit();
        LightsInit();
		PBRInit();
        LoadShader();
        LoadTexture();
    }

    PBRTest::~PBRTest()
    {

    }

    void PBRTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
        m_view = cam.GetViewMatrix();
    }

    void PBRTest::OnRender()
    {
        Renderer render;
        //光源
        {
            Shader_Light->Bind();
            Shader_Light->SetUniformMat4f("u_Projection", m_proj);
            Shader_Light->SetUniformMat4f("u_View", m_view);
            m_model_light0 = glm::translate(glm::mat4(1.0f), glm::vec3(temp, temp, lightDistance));
            Shader_Light->SetUniformMat4f("u_Model", m_model_light0);
            render.Draw(*VAO_Light, *IBO_Light, *Shader_Light);
            m_model_light1 = glm::translate(glm::mat4(1.0f), glm::vec3(-temp, temp, lightDistance));
            Shader_Light->SetUniformMat4f("u_Model", m_model_light1);
            render.Draw(*VAO_Light, *IBO_Light, *Shader_Light);
            m_model_light2 = glm::translate(glm::mat4(1.0f), glm::vec3(temp, -temp, lightDistance));
            Shader_Light->SetUniformMat4f("u_Model", m_model_light2);
            render.Draw(*VAO_Light, *IBO_Light, *Shader_Light);
            m_model_light3 = glm::translate(glm::mat4(1.0f), glm::vec3(-temp, -temp, lightDistance));
            Shader_Light->SetUniformMat4f("u_Model", m_model_light3);
            render.Draw(*VAO_Light, *IBO_Light, *Shader_Light);
        }
		//PBR球体
        {
			m_model_PBR = glm::translate(glm::mat4(1.0f), PBR_Position);
            //m_model_PBR = m_model_PBR * glm::scale(glm::mat4(1.0f), glm::vec3(80.0f));

			Shader_PBR->Bind();
			Shader_PBR->SetUniformMat4f("projection", m_proj);
			Shader_PBR->SetUniformMat4f("view", m_view);
			Shader_PBR->SetUniformMat4f("model", m_model_PBR);
			Shader_PBR->SetUniformMat3f("normalMatrix", glm::mat3(glm::transpose(glm::inverse(glm::mat3(m_model_PBR)))));
			albedoMap->Bind(0);
			Shader_PBR->SetUniform1i("albedoMap", 0);
			normalMap->Bind(1);
			Shader_PBR->SetUniform1i("normalMap", 1);
			metallicMap->Bind(2);
			Shader_PBR->SetUniform1i("metallicMap", 2);
			roughnessMap->Bind(3);
			Shader_PBR->SetUniform1i("roughnessMap", 3);
			aoMap->Bind(4);
			Shader_PBR->SetUniform1i("aoMap", 4);
			Shader_PBR->SetUniform1i("lightCount", lightCount);

            std::string lightPosStr;
            std::string lightColors;
            lightPosStr = "lightPositions[" + std::to_string(0) + "]";
            lightColors = "lightColors[" + std::to_string(0) + "]";
            Shader_PBR->SetUniformVec3f(lightPosStr, glm::vec3(temp, temp, lightDistance));
            Shader_PBR->SetUniformVec3f(lightColors, m_color_light0);
            lightPosStr = "lightPositions[" + std::to_string(1) + "]";
            lightColors = "lightColors[" + std::to_string(1) + "]";
            Shader_PBR->SetUniformVec3f(lightPosStr, glm::vec3(-temp, temp, lightDistance));
            Shader_PBR->SetUniformVec3f(lightColors, m_color_light1);
            lightPosStr = "lightPositions[" + std::to_string(2) + "]";
            lightColors = "lightColors[" + std::to_string(2) + "]";
            Shader_PBR->SetUniformVec3f(lightPosStr, glm::vec3(temp, -temp, lightDistance));
            Shader_PBR->SetUniformVec3f(lightColors, m_color_light2);
            lightPosStr = "lightPositions[" + std::to_string(3) + "]";
            lightColors = "lightColors[" + std::to_string(3) + "]";
            Shader_PBR->SetUniformVec3f(lightPosStr, glm::vec3(-temp, -temp, lightDistance));
            Shader_PBR->SetUniformVec3f(lightColors, m_color_light3);

			Shader_PBR->SetUniform1f("metallicOverride", metallic);
			Shader_PBR->SetUniform1f("roughnessOverride", roughness);
			Shader_PBR->SetUniformVec3f("camPos", cam.GetPosition());
			render.Draw(*VAO_PBR, *IBO_PBR, *Shader_PBR);
        }
    }

    void PBRTest::LoadTexture()
    {
		albedoMap = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/rusted_iron/albedo.png");
        normalMap = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/rusted_iron/normal.png");
		metallicMap = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/rusted_iron/metallic.png");
		roughnessMap = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/rusted_iron/roughness.png");
		aoMap = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/rusted_iron/ao.png");
    }

    void PBRTest::LoadShader()
    {
        Shader_Light = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/PBRShaders/Lights.shader");
		Shader_PBR = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/PBRShaders/PBR.shader");
	}

    void PBRTest::LoadModel()
    {
    }

    void PBRTest::OnImGuiRender()
    {
        ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate); // 显示帧率，保留1位小数
		ImGui::SliderFloat("Metallic Override", &metallic, 0.0f, 1.0f);
		ImGui::SliderFloat("Roughness Override", &roughness, 0.0f, 1.0f);
		ImGui::SliderFloat("Light Distance", &lightDistance, 0.1f, 2.0f);
		ImGui::SliderFloat3("PBR Position", &PBR_Position.x, -2.0f, 2.0f);
    }

    void PBRTest::CameraInit()
    {
		cam.SetCameraPos(cameraPos);
        cam.SetMovementSpeed(0.5f);
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();
    }

    void PBRTest::LightsInit()
    {
        BasicModel::BasicModels basicModels;
        std::array<BasicModel::Vertex, 24> cubeVertexs = basicModels.CreateCubeVertexs(1.0f);
        std::vector<unsigned int> cubeIndices = basicModels.CreateCubeIndices();
        VAO_Light = std::make_unique<VertexArray>();
        VBO_Light = std::make_unique<VertexBuffer>(cubeVertexs.data(), sizeof(BasicModel::Vertex) * cubeVertexs.size());
        IBO_Light = std::make_unique<IndexBuffer>(cubeIndices.data(), cubeIndices.size());
        VertexBufferLayout layout_Light;
        layout_Light.Push<float>(3);
        layout_Light.Push<float>(3);
        layout_Light.Push<float>(2);
        VAO_Light->AddBuffer(*VBO_Light, layout_Light);

        ////随机生成光源位置
        //srand(glfwGetTime());
        //float radius = 60.0;
        //float offset = 1.0;
        //for (int i = 0; i < lightCount; i++)
        //{
        //    glm::mat4 model(1.0f);
        //    // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
        //    float angle = (float)i / (float)lightCount * 360.0f;
        //    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        //    float x = sin(angle) * radius + displacement;
        //    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        //    float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        //    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        //    float z = cos(angle) * radius + displacement;
        //    model = glm::translate(model, glm::vec3(x, y, z));
        //
        //    float scale = 5.0f;
        //    model = glm::scale(model, glm::vec3(scale));
        //
        //    // 4. 添加到矩阵的数组中
        //    instanceMatrices.push_back(model);
        //}
        //VBO_Light_Instance = std::make_unique<VertexBuffer>(instanceMatrices.data(), sizeof(glm::mat4) * instanceMatrices.size(), false);
        //VAO_Light->AddInstanceBuffer(*VBO_Light_Instance);
    }

    void PBRTest::PBRInit()
    {
		BasicModel::BasicModels basicModels;
		std::vector<BasicModel::Vertex> sphereVertexs = basicModels.CreateSphereVertices(1.0f, 64, 64);
		std::vector<unsigned int> sphereIndices = basicModels.CreateSphereIndices(64, 64);
		VAO_PBR = std::make_unique<VertexArray>();
		VBO_PBR = std::make_unique<VertexBuffer>(sphereVertexs.data(), sizeof(BasicModel::Vertex) * sphereVertexs.size());
		IBO_PBR = std::make_unique<IndexBuffer>(sphereIndices.data(), sphereIndices.size());
		VertexBufferLayout layout_PBR;
		layout_PBR.Push<float>(3);
		layout_PBR.Push<float>(3);
		layout_PBR.Push<float>(2);
		VAO_PBR->AddBuffer(*VBO_PBR, layout_PBR);
    }
}

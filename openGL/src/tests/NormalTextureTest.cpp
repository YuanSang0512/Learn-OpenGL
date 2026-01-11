#include "NormalTextureTest.h"

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"

namespace test
{
    NormalTextureTest::NormalTextureTest()
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));
        CameraInit();
        LoadTexture();
		LoadShader();

        BasicModel::BasicModels basicModels;
		//生成墙面模型数据
		std::array<BasicModel::Vertex, 4> planeVertexs = basicModels.CreatePlaneVertexs({ 100.0f,100.0f }, { 0.0f,1.0f,0.0f });
		std::vector<unsigned int> planeIndices = basicModels.CreatePlaneIndices();

		va_wall = std::make_unique<VertexArray>();
		vb_wall = std::make_unique<VertexBuffer>(planeVertexs.data(), planeVertexs.size() * sizeof(BasicModel::Vertex));
		ib_wall = std::make_unique<IndexBuffer>(planeIndices.data(), planeIndices.size());
        VertexBufferLayout layout;
		layout.Push<float>(3);//position
		layout.Push<float>(3);//normal
		layout.Push<float>(2);//texCoord
		va_wall->AddBuffer(*vb_wall, layout);

		//生成光源模型数据
		std::array<BasicModel::Vertex, 24> lightVertexs = basicModels.CreateCubeVertexs(10);
		std::vector<unsigned int> lightIndices = basicModels.CreateCubeIndices();

		va_light = std::make_unique<VertexArray>();
		vb_light = std::make_unique<VertexBuffer>(lightVertexs.data(), lightVertexs.size() * sizeof(BasicModel::Vertex));
		ib_light = std::make_unique<IndexBuffer>(lightIndices.data(), lightIndices.size());
		va_light->AddBuffer(*vb_light, layout);
    }

    NormalTextureTest::~NormalTextureTest()
    {

    }

    void NormalTextureTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
        m_view = cam.GetViewMatrix();


        m_model_light = glm::mat4(1.0f);
        m_model_light = glm::translate(m_model_light, lightPos);
        
    }

    void NormalTextureTest::OnRender()
    {
        Renderer render;
        //绘制墙面
        {
            shader_wall->Bind();
            shader_wall->SetUniformMat4f("u_Projection", m_proj);
            shader_wall->SetUniformMat4f("u_View", m_view);
            shader_wall->SetUniformMat4f("u_Model", m_model_wall);

            ambientTex_wall->Bind(0);
            shader_wall->SetUniform1i("u_Material.texture_diffuse1", 0);
            normalTex_wall->Bind(1);
            shader_wall->SetUniform1i("u_Material.texture_normal1", 1);
			shader_wall->SetUniform1f("u_Material.shininess", shininess);

			shader_wall->SetUniformVec3f("u_Light.lightPos", lightPos);
			shader_wall->SetUniformVec3f("u_Light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			shader_wall->SetUniformVec3f("u_Light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));

			shader_wall->SetUniformVec3f("u_CamPos", cam.GetPosition());

            render.Draw(*va_wall, *ib_wall, *shader_wall);
		}

        //绘制光源
        {
            shader_light->Bind();

            shader_light->SetUniformMat4f("u_Projection", m_proj);
            shader_light->SetUniformMat4f("u_View", m_view);
            shader_light->SetUniformMat4f("u_Model", m_model_light);

            render.Draw(*va_light, *ib_light, *shader_light);
		}
    }

    void NormalTextureTest::LoadModel()
    {
    }

    void NormalTextureTest::LoadTexture()
    {
        ambientTex_wall = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/wall/brickwall.jpg", TextureType::TEXTURE_2D);
        normalTex_wall = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/wall/brickwall_normal.jpg", TextureType::TEXTURE_2D);
    }

    void NormalTextureTest::LoadShader()
    {
		shader_wall = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/NormalTextureShaders/Wall.shader");
		shader_light = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/NormalTextureShaders/Light.shader");
    }

    void NormalTextureTest::OnImGuiRender()
    {
        ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate); // 显示帧率，保留1位小数
		ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f);
		ImGui::SliderFloat3("Light Position", &lightPos[0], -100.0f, 100.0f);
    }

    void NormalTextureTest::CameraInit()
    {
		cam.SetCameraPos(camPos);
        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)1500 / 1500; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 15000.0f;
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();
        m_model_wall = glm::mat4(1.0f);

        float scale = 1.0f;
		m_model_light = glm::mat4(1.0f);
		m_model_light = glm::translate(m_model_light, lightPos);
		m_model_light = glm::scale(m_model_light, glm::vec3(scale, scale, scale));
    }
}
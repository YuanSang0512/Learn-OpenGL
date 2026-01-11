#include "InstanceTest.h"

#include <array>

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"

namespace test
{
    InstanceTest::InstanceTest()
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

        BasicModel::BasicModels basicModels;
        //生成天空盒模型数据
        std::array<BasicModel::skyVertex, 24> vertices_skyBox = basicModels.CreateSkyBoxVertexs();
        std::vector<unsigned int> indices_skyBox = basicModels.CreateSkyBoxIndices();

        //初始化天空盒数据
        m_SkyBoxVAO = std::make_unique<VertexArray>();
        m_SkyBoxVBO = std::make_unique<VertexBuffer>(vertices_skyBox.data(), 24 * sizeof(BasicModel::Vertex), false);
        VertexBufferLayout SkyBoxLayout;
        SkyBoxLayout.Push<float>(3);//Position
        m_SkyBoxVAO->AddBuffer(*m_SkyBoxVBO, SkyBoxLayout);
        m_SkyBoxEBO = std::make_unique<IndexBuffer>(indices_skyBox.data(), 36);
        m_SkyBoxTexture = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/skybox/", TextureType::TEXTURE_CUBE_MAP);

        //初始化陨石数据
		unsigned int amount = 100000;
        srand(glfwGetTime()); // 初始化随机种子    
        float radius = 300.0;
        float offset = 25.5f;
        for (unsigned int i = 0; i < amount; i++)
        {
            glm::mat4 model(1.0f);
            // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
            float angle = (float)i / (float)amount * 360.0f;
            float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. 缩放：在 0.05 和 0.25f 之间缩放
            float scale = (rand() % 20) / 100.0f + 0.05;
            model = glm::scale(model, glm::vec3(scale));

            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. 添加到矩阵的数组中
            modelMatrices.push_back(model);
        }
		LoadModel();

        //加载Shader文件
        m_SkyBoxShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/InstanceTestShaders/SkyBox.shader");
        m_NormalShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/InstanceTestShaders/Model_Normal.shader");
        m_RockShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/InstanceTestShaders/Rock.shader");
    }

    InstanceTest::~InstanceTest()
    {

    }

    void InstanceTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
    }

    void InstanceTest::OnRender()
    {
        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)1500 / 1500; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 15000.0f;
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();

        Renderer render;

        //绘制行星
        {
            m_NormalShader->Bind();
            m_model = glm::mat4(1.0f);
            m_model = glm::translate(m_model, glm::vec3(0.0f, 0.0f, 0.0f));
            m_scale = 10.0f;
            m_model = glm::scale(m_model, glm::vec3(m_scale, m_scale, m_scale));
            m_NormalShader->SetUniformMat4f("u_Projection", m_proj);
            m_NormalShader->SetUniformMat4f("u_View", m_view);
            m_NormalShader->SetUniformMat4f("u_Model", m_model);

            m_NormalShader->SetUniform1f("u_Material.shininess", shininess);

            m_NormalShader->SetUniformVec3f("u_Light.lightPos", lightPos);
            m_NormalShader->SetUniformVec3f("u_Light.ambient", ambientLight);
            m_NormalShader->SetUniformVec3f("u_Light.diffuse", diffuseLight);
            m_NormalShader->SetUniformVec3f("u_Light.specular", specularLight);

            planet.Draw(*m_NormalShader);
		}

        //绘制陨石
        {
            m_RockShader->Bind();
            m_model = glm::mat4(1.0f);
            //m_scale = 40.0f;
            //m_model = glm::scale(m_model, glm::vec3(m_scale, m_scale, m_scale));
            m_RockShader->SetUniformMat4f("u_Projection", m_proj);
            m_RockShader->SetUniformMat4f("u_View", m_view);
            m_RockShader->SetUniformMat4f("u_Model", m_model);

            rock.Draw(*m_RockShader, modelMatrices);
		}

    }

    void InstanceTest::LoadModel()
    {
        rock = Model("E:/VS project/openGL/openGL/res/models/rock/rock.obj", RendererType::Multiple, modelMatrices);
        planet = Model("E:/VS project/openGL/openGL/res/models/planet/planet.obj");
    }


    void InstanceTest::OnImGuiRender()
    {
        ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate); // 显示帧率，保留1位小数
    }
}
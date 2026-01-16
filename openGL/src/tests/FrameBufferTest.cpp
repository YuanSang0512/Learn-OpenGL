#include "FrameBufferTest.h"

#include <array>

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"
//#include <GL/glew.h>
//#include "GLFW/glfw3.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    FrameBufferTest::FrameBufferTest()
        :objectPos(0, 0, 0), modelPos(0, 300, 0), m_CubeColor(1, 1, 0), shininess(32),
        ambientLight(0.4, 0.4, 0.4), diffuseLight(0.4, 0.4, 0.4), specularLight(0.5, 0.5, 0.5)
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

        //生成立方体模型数据
        std::array<BasicModel::Vertex, 24> vertices_object = basicModels.CreateCubeVertexs(300.0f);
        std::vector<unsigned int> indices_object = basicModels.CreateCubeIndices();

        //生成屏幕平面顶点数据
        std::vector<float> vertices_Scr = basicModels.GetScrVertex();

        //帧缓冲
        m_FBO = std::make_unique<FrameBuffer>();
        m_FOBTexture = std::make_unique<Texture>(" ", TextureType::TEXTURE_FRAMEBUFFER);
        m_RBO = std::make_unique<RenderBuffer>();
        m_FBO->Check();
        
        //初始化屏幕数据
        m_ScrVAO = std::make_unique<VertexArray>();
        m_ScrVBO = std::make_unique<VertexBuffer>(vertices_Scr.data(), vertices_Scr.size() * sizeof(float), false);
        VertexBufferLayout ScrLayout;
        ScrLayout.Push<float>(2);//Position
        ScrLayout.Push<float>(2);//TexCoord
        m_ScrVAO->AddBuffer(*m_ScrVBO, ScrLayout);
        
        //初始化天空盒数据
		m_SkyBoxVAO = std::make_unique<VertexArray>();
		m_SkyBoxVBO = std::make_unique<VertexBuffer>(vertices_skyBox.data(), 24 * sizeof(BasicModel::Vertex), false);
        VertexBufferLayout SkyBoxLayout;
        SkyBoxLayout.Push<float>(3);//Position
        m_SkyBoxVAO->AddBuffer(*m_SkyBoxVBO, SkyBoxLayout);
        m_SkyBoxEBO = std::make_unique<IndexBuffer>(indices_skyBox.data(), 36);
		m_SkyBoxTexture = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/skybox/", TextureType::TEXTURE_CUBE_MAP);
        
        //初始化模型数据
        m_ModelVAO = std::make_unique<VertexArray>();
        m_ModelVBO = std::make_unique<VertexBuffer>(vertices_object.data(), 24 * sizeof(BasicModel::Vertex), false);
        VertexBufferLayout ObjectLayout;
        ObjectLayout.Push<float>(3);//Position
        ObjectLayout.Push<float>(3);//Normal
        ObjectLayout.Push<float>(2);//TexCoord
        m_ModelVAO->AddBuffer(*m_ModelVBO, ObjectLayout);
        m_ModelEBO = std::make_unique<IndexBuffer>(indices_object.data(), 36);
        m_ModelTexture = std::make_unique<Texture>("E:/VS project/openGL/openGL/res/textures/container2.png", TextureType::TEXTURE_2D);
        
        //加载Shader文件
        m_SkyBoxShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/SkyBox.shader");
        m_PassShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/FrameBufferShaders/Pass.shader");
        m_ReflectShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/CubeReflex.shader");
        m_NormalShader = std::make_unique<Shader>("E:/VS project/openGL/openGL/res/shaders/ModelTest.shader");
    }

    FrameBufferTest::~FrameBufferTest()
    {

    }

    void FrameBufferTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
    }

    void FrameBufferTest::OnRender()
    {
        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)1500 / 1500; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 5000.0f;
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();

        Renderer render;

        //绘制天空盒
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST); // 开启深度测试，保证3D物体遮挡关系
            glDepthFunc(GL_LEQUAL);
            glDepthMask(GL_FALSE);
            m_SkyBoxShader->Bind();
            m_SkyBoxTexture->Bind(0);
            m_SkyBoxShader->SetUniform1i("u_Skybox", 0);
            m_SkyBoxShader->SetUniformMat4f("u_Projection", m_proj);
            glm::mat4 view = glm::mat4(glm::mat3(m_view));
            m_SkyBoxShader->SetUniformMat4f("u_View", view);

            render.Draw(*m_SkyBoxVAO, *m_SkyBoxEBO, *m_SkyBoxShader);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }

        m_FBO->Bind();

		//绘制立方体
        if (Cube)
        {
            m_ReflectShader->Bind();
            m_model = glm::translate(glm::mat4(1.0f), objectPos);
            m_model = glm::rotate(m_model, glm::radians(0.0f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(1.0f));

            m_ReflectShader->SetUniformMat4f("model", m_model);
            m_ReflectShader->SetUniformMat4f("view", m_view);
            m_ReflectShader->SetUniformMat4f("projection", m_proj);

            m_ReflectShader->SetUniformVec3f("u_CameraPos", cam.GetPosition());
            m_ReflectShader->SetUniform1i("u_Skybox", 0);

            render.Draw(*m_ModelVAO, *m_ModelEBO, *m_ReflectShader);
        }

        //绘制模型
        {
            m_ReflectShader->Bind();
            m_model = glm::translate(glm::mat4(1.0f), modelPos);
            m_model = glm::rotate(m_model, glm::radians(0.0f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(10.0f));

            m_ReflectShader->SetUniformMat4f("model", m_model);
            m_ReflectShader->SetUniformMat4f("view", m_view);
            m_ReflectShader->SetUniformMat4f("projection", m_proj);

            m_ReflectShader->SetUniformVec3f("u_CameraPos", cam.GetPosition());
            m_ReflectShader->SetUniform1i("u_Skybox", 0);

            backpack.Draw(*m_ReflectShader);
        }

        {
            m_NormalShader->Bind();
            glm::vec3 offsetPosition(50, 0, 0);
            m_model = glm::translate(glm::mat4(1.0f), modelPos + offsetPosition);
            m_model = glm::rotate(m_model, glm::radians(0.0f), glm::vec3(0, 1, 0));
            m_model = glm::scale(m_model, glm::vec3(10.0f));
            m_NormalShader->SetUniformMat4f("u_Model", m_model);
            m_NormalShader->SetUniformMat4f("u_View", m_view);
            m_NormalShader->SetUniformMat4f("u_Projection", m_proj);

            m_NormalShader->SetUniform1f("u_Material.shininess", shininess);

            m_NormalShader->SetUniformVec3f("u_Light.lightPos", glm::vec3(500, 500, 200));
            m_NormalShader->SetUniformVec3f("u_Light.ambient", ambientLight);
            m_NormalShader->SetUniformVec3f("u_Light.diffuse", diffuseLight);
            m_NormalShader->SetUniformVec3f("u_Light.specular", specularLight);

            m_NormalShader->SetUniformVec3f("u_CamPos", cam.GetPosition());

            backpack.Draw(*m_NormalShader);
        }

        m_FBO->Unbind();
        //绘制屏幕
        {
            m_PassShader->Bind();
            m_FOBTexture->Bind(0);
            m_PassShader->SetUniform1i("screenTexture", 0);
            render.Draw(*m_ScrVAO, *m_PassShader);

        }
    }

    void FrameBufferTest::OnImGuiRender()
    {
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate); // 显示帧率，保留1位小数
        ImGui::Checkbox("Cube", &Cube);
    }
}
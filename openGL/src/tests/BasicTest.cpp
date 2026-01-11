#include "BasicTest.h"

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"

namespace test
{
    TestName::TestName()
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));
        CameraInit();

        //BasicModel::BasicModels basicModels;

        //加载Shader文件
    }

    TestName::~TestName()
    {

    }

    void TestName::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
    }

    void TestName::OnRender()
    {
        //Renderer render;

    }

    void TestName::LoadModel()
    {
    }


    void TestName::OnImGuiRender()
    {
        ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate); // 显示帧率，保留1位小数
    }

    void TestName::CameraInit()
    {
        float fov = glm::radians(80.0f); // 视野角度
        float aspect = (float)1500 / 1500; // 屏幕宽高比
        float nearPlane = 0.1f;
        float farPlane = 15000.0f;
        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();
    }
}
#include "CubeMapTest.h"

#include <array>

#include "BasicModels.h"
#include "InputProcess.h"

#include "imgui/imgui.h"
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    CubeMapTest::CubeMapTest()
        :objectPos(0, 0, 0), m_CubeColor(1, 1, 0), shininess(32),
        ambientLight(0.4, 0.4, 0.4), diffuseLight(0.4, 0.4, 0.4), specularLight(0.5, 0.5, 0.5)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));

		//生成立方体模型数据
        BasicModels basicModels;
        std::array<vertex, 24> vertices_object;
        vertex* buffer = vertices_object.data();
        buffer = basicModels.CreateCubeVertexs(buffer, 300.0f);
        std::vector<unsigned int> indices_object(36);
        indices_object = basicModels.CreateCubeIndices(indices_object);

        
    }

    CubeMapTest::~CubeMapTest()
    {

    }

    void CubeMapTest::OnUpdate(float deltaTime)
    {
        cam.CameraUpdate();
    }

    void CubeMapTest::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


        m_proj = cam.GetProjectionMatrix(aspect, nearPlane, farPlane);
        m_view = cam.GetViewMatrix();

        Renderer render;

    }

    void CubeMapTest::OnImGuiRender()
    {
        ImGui::Text("Transformations");
        ImGui::SliderFloat("radius", &radius, 0.0f, 500.0f);
        ImGui::SliderFloat3("objectPos", &objectPos.x, -900.0f, 900.0f);
        ImGui::SliderFloat3("CubeColor", &m_CubeColor.x, 0.0f, 1.0f);
    }
}
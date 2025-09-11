#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "InputProcess.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/ColorfulCube.h"
#include "tests/TestMultiTexture.h"
#include "tests/DynamicVBO.h"
#include "tests/VertexIndex.h"
#include "tests/CameraTest.h"
#include "tests/LightTest.h"
#include "tests/LightingMaps.h"
#include "tests/ModelTest.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"
using namespace std;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    mouseState.scroll_y = yoffset;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << "Mouse moved to (" << xpos << ", " << ypos << ")" << std::endl;
    //std::cout << mouseState.diff_x << ", " << mouseState.diff_y << std::endl;
	mouseState.diff_x = xpos - mouseState.x;
	mouseState.diff_y = ypos - mouseState.y;
	mouseState.x = xpos;
	mouseState.y = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_W) 
        keyState.w = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A) 
        keyState.a = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S) 
        keyState.s = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D) 
        keyState.d = (action != GLFW_RELEASE);
    if(key == GLFW_KEY_LEFT_SHIFT)
		keyState.shift = (action != GLFW_RELEASE);
    if(key == GLFW_KEY_SPACE)
		keyState.space = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        keyState.table = !keyState.table;
        if (keyState.table) {
            // �������ص�����
            glfwSetCursorPosCallback(window, cursor_position_callback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else {
            // �������ص�����
            glfwSetCursorPosCallback(window, nullptr);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

int main(void)
{
    #pragma region ��ʼ��

    //��ʼ��glfw
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    //ָ��OpenGL�汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //����GLFW����
    GLFWwindow* window = glfwCreateWindow(1500, 1500, "Open_GL", NULL, NULL);
    //����������
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    //���ô�ֱͬ��
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
#pragma endregion

    {
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));//���û��

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
        testMenu->RegisterTest<test::ColorfulCube>("ColorfulCube");
        testMenu->RegisterTest<test::TestMultiTexture>("TestMultiTexture");
        testMenu->RegisterTest<test::DynamicVBO>("DynamicVBO");
        testMenu->RegisterTest<test::VertexIndex>("VertexIndex");
        testMenu->RegisterTest<test::CameraTest>("CameraTest");
        testMenu->RegisterTest<test::LightTest>("LightTest");
        testMenu->RegisterTest<test::LightingMaps>("LightingMaps");
        testMenu->RegisterTest<test::ModelTest>("ModelTest");

        //�ص�����
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetScrollCallback(window, scroll_callback);

        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            
            ImGui_ImplGlfwGL3_NewFrame();

            //���������С
            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = 2.0f;

            if (currentTest)
            {
                glfwPollEvents(); 
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();

                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();

                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
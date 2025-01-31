#include "SquaresScene.h"
using Pekan::SquaresScene;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const unsigned WINDOW_WIDTH = 800;
const unsigned WINDOW_HEIGHT = 600;

const char* glsl_version = "#version 330 core";

GLFWwindow* setupWindow()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "ERROR: GLFW failed to initialize." << std::endl;
        return nullptr;
    }

    // Set window hint for OpenGL 4.3 and OpenGL Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pekan v0.0", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "ERROR: Failed to create a window with GLFW." << std::endl;
        glfwTerminate();
        return nullptr;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window);
    // Enalbe VSync
    glfwSwapInterval(1);

    return window;
}

void cleanupWindow(GLFWwindow* window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool loadOpenGL()
{
    // Load OpenGL function pointers with GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: GLAD failed to load OpenGL functions." << std::endl;
        return false;
    }
    // Set OpenGL viewport's resolution to be the same as window's resolution
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Log OpenGL version
    std::cout << "Successfully loaded OpenGL " << glGetString(GL_VERSION) << std::endl;

    return true;
}

void setupImGui(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void drawImGuiDemo(GLFWwindow* window, ImVec4& clearColor)
{
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
    {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(200, 70));
    ImGui::Begin("Pekan");

    ImGui::Text("Background Color");
    ImGui::ColorEdit3("", (float*)&clearColor);

    ImGui::End();

    ImGui::Render();
}

void renderImGuiDemo()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void cleanupImGuiDemo()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main(void)
{
    GLFWwindow* window = setupWindow();
    if (window == nullptr)
    {
        return -1;
    }
    if (!loadOpenGL())
    {
        return -1;
    }
    setupImGui(window);

    SquaresScene squaresScene;
    if (!squaresScene.init())
    {
        return -1;
    }

    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        drawImGuiDemo(window, clearColor);
        
        int windowWidth, windowHeight;
        glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);

        squaresScene.render();
        renderImGuiDemo();

        glfwSwapBuffers(window);
    }

    squaresScene.cleanup();
    cleanupImGuiDemo();

    cleanupWindow(window);
    return 0;
}
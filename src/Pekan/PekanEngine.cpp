#include "PekanEngine.h"
#define PK_FILENAME "PekanEngine.cpp"
#include "Logger/PekanLogger.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Pekan
{
    const int DEFAULT_WINDOW_WIDTH = 1280;
    const int DEFAULT_WINDOW_HEIGHT = 720;

    const int DEFAULT_OPENGL_VERSION_MAJOR = 4;
    const int DEFAULT_OPENGL_VERSION_MINOR = 3;
    const char* DEFAULT_GLSL_VERSION = "#version 330 core";

    const char* DEFAULT_WINDOW_TITLE = "Pekan v0.0";

    bool PekanEngine::init()
    {
        if (!createWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT))
        {
            return false;
        }
        if (!loadOpenGL())
        {
            return false;
        }
        if (!initImGui())
        {
            return false;
        }
        return true;
    }

    void PekanEngine::exit()
    {
        if (window == nullptr)
        {
            PK_LOG_ERRORF("Trying to exit engine but engine is not yet initialized.");
            return;
        }
        exitImGui();
        destroyWindow();
    }

    int PekanEngine::getWindowWidth()
    {
        return DEFAULT_WINDOW_WIDTH;
    }

    int PekanEngine::getWindowHeight()
    {
        return DEFAULT_WINDOW_HEIGHT;
    }

    bool PekanEngine::createWindow(int width, int height)
    {
        if (!glfwInit())
        {
            PK_LOG_ERRORF("Failed to initialize GLFW.");
            return false;
        }

        // Set window hint for OpenGL version and OpenGL Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, DEFAULT_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, DEFAULT_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a GLFW window
        window = glfwCreateWindow(width, height, DEFAULT_WINDOW_TITLE, nullptr, nullptr);
        if (window == nullptr)
        {
            PK_LOG_ERRORF("Failed to create a window with GLFW.");
            glfwTerminate();
            return false;
        }
        // Make the window's context current
        glfwMakeContextCurrent(window);
        // Enalbe VSync
        glfwSwapInterval(1);

        return true;
    }

    void PekanEngine::destroyWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }

    bool PekanEngine::loadOpenGL() const
    {
        // Load OpenGL function pointers with GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            PK_LOG_ERRORF("Failed to load OpenGL function pointers with GLAD");
            return false;
        }
        // Set OpenGL viewport's resolution to be the same as window's resolution
        glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

        // Log OpenGL version
        PK_LOG_INFO("Successfully loaded OpenGL " << glGetString(GL_VERSION));

        return true;
    }

    bool PekanEngine::initImGui() const
    {
        if (window == nullptr)
        {
            PK_LOG_ERRORF("Trying to initialize ImGui but engine is not yet initialized.");
            return false;
        }
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        // Setup ImGui style
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
        ImGui_ImplOpenGL3_Init(DEFAULT_GLSL_VERSION);

        return true;
    }

    void PekanEngine::renderImGui() const
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void PekanEngine::exitImGui() const
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace Pekan
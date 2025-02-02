#include "PekanEngine.h"

#include "SquaresScene.h"
#include "SquaresGui.h"
using Pekan::SquaresScene;
using Pekan::SquaresGui;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Pekan
{
    const unsigned DEFAULT_WINDOW_WIDTH = 1280;
    const unsigned DEFAULT_WINDOW_HEIGHT = 720;

    const char* DEFAULT_GLSL_VERSION = "#version 330 core";

	bool PekanEngine::init()
	{
        // Create a window
        if (!createWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT))
        {
            return false;
        }
        if (window == nullptr)
        {
            return false;
        }
        // Load OpenGL function pointers
        if (!loadOpenGL())
        {
            return false;
        }
        // Initialize ImGui library
        if (!initImGui())
        {
            return false;
        }
        return true;
	}

    void PekanEngine::cleanup()
    {
        if (window == nullptr)
        {
            std::cout << "ERROR in PekanEngine: Trying to clean up engine but engine is not yet initialized." << std::endl;
            return;
        }
        cleanupImGui();
        cleanupWindow();
    }

    bool PekanEngine::runExercise0000() const
    {
        if (window == nullptr)
        {
            std::cout << "ERROR in PekanEngine: Trying to run exercise0000 but engine is not yet initialized." << std::endl;
            return false;
        }
        SquaresScene squaresScene;
        if (!squaresScene.init(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT))
        {
            return false;
        }

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            int windowWidth, windowHeight;
            glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            squaresScene.render();
            SquaresGui::render(window, squaresScene);
            renderImGui();

            glfwSwapBuffers(window);
        }

        squaresScene.cleanup();
        return true;
    }

    bool PekanEngine::createWindow(int width, int height)
	{
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cout << "ERROR in PekanEngine: GLFW failed to initialize." << std::endl;
            return false;
        }

        // Set window hint for OpenGL 4.3 and OpenGL Core Profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // Create a window
        window = glfwCreateWindow(width, height, "Pekan v0.0", nullptr, nullptr);
        if (window == nullptr)
        {
            std::cout << "ERROR in PekanEngine: Failed to create a window with GLFW." << std::endl;
            glfwTerminate();
            return false;
        }
        // Make the window's context current
        glfwMakeContextCurrent(window);
        // Enalbe VSync
        glfwSwapInterval(1);

        return true;
	}

    void PekanEngine::cleanupWindow()
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
            std::cout << "ERROR in PekanEngine: GLAD failed to load OpenGL functions." << std::endl;
            return false;
        }
        // Set OpenGL viewport's resolution to be the same as window's resolution
        glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

        // Log OpenGL version
        std::cout << "Successfully loaded OpenGL " << glGetString(GL_VERSION) << std::endl;

        return true;
    }

    bool PekanEngine::initImGui() const
    {
        if (window == nullptr)
        {
            std::cout << "ERROR in PekanEngine: Trying to initialize ImGui but engine is not yet initialized." << std::endl;
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

    void PekanEngine::cleanupImGui() const
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace Pekan
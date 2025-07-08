#include "PekanEngine.h"

#include "PekanLogger.h"
#include "PekanApplication.h"
#include "SubsystemManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define PK_GLSL_VERSION "#version 330 core"

namespace Pekan
{

    Window PekanEngine::s_window;
    PekanApplication* PekanEngine::s_application = nullptr;
    bool PekanEngine::isInitialized = false;

    bool PekanEngine::init(PekanApplication* application)
    {
        if (isInitialized)
        {
            PK_LOG_ERROR("Trying to initialize the engine more than once.", "Pekan");
            return false;
        }
        if (s_application != nullptr)
        {
            PK_LOG_ERROR("Multiple applications are trying to initialize the engine. Pekan supports only one application at a time.", "Pekan");
            return false;
        }
        if (application == nullptr)
        {
            PK_LOG_ERROR("A null application is given when trying to initialize the engine.", "Pekan");
            return false;
        }
        s_application = application;

        const ApplicationProperties properties = application->getProperties();
        if (!s_window.create(properties.windowProperties))
        {
            return false;
        }
        if (!initImGui())
        {
            return false;
        }

        SubsystemManager::initAll();

        isInitialized = true;
        return true;
    }

    void PekanEngine::exit()
    {
        if (!isInitialized)
        {
            PK_LOG_ERROR("Trying to exit engine but engine is not yet initialized.", "Pekan");
            return;
        }
        exitImGui();
        s_window.destroy();

        SubsystemManager::exitAll();

        isInitialized = false;
    }

    bool PekanEngine::isKeyPressed(KeyCode key)
    {
        return s_window.isKeyPressed(key);
    }

    bool PekanEngine::isKeyReleased(KeyCode key)
    {
        return s_window.isKeyReleased(key);
    }

    bool PekanEngine::isKeyRepeating(KeyCode key)
    {
        return s_window.isKeyRepeating(key);
    }

    glm::vec2 PekanEngine::getMousePosition()
    {
        return s_window.getMousePosition();
    }

    bool PekanEngine::isMouseButtonPressed(MouseButton button)
    {
        return s_window.isMouseButtonPressed(button);
    }

    bool PekanEngine::isMouseButtonReleased(MouseButton button)
    {
        return s_window.isMouseButtonReleased(button);
    }

    glm::ivec2 PekanEngine::getWindowSize()
    {
        return s_window.getSize();
    }

    bool PekanEngine::initImGui()
    {
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
        ImGui_ImplGlfw_InitForOpenGL(s_window.getGlfwWindow(), true);
        ImGui_ImplOpenGL3_Init(PK_GLSL_VERSION);

        // Scale up widgets and font by a factor of 2
        style.ScaleAllSizes(1.3f);
        io.FontGlobalScale = 1.05f;

        return true;
    }

    void PekanEngine::exitImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace Pekan
#include "GUISystem.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define PK_GLSL_VERSION "#version 330 core"

namespace Pekan
{
namespace GUI
{

    int GUISystem::m_nextWidgetId = 0;

	static GUISystem g_guiSystem;

	void GUISystem::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_guiSystem);
	}

	GUISystem* GUISystem::getInstance()
	{
		return &g_guiSystem;
	}

    int GUISystem::generateWidgetId()
    {
        return m_nextWidgetId++;
    }

    bool GUISystem::init()
    {
        if (!initImGui())
        {
            PK_LOG_ERROR("Failed to initialize ImGui when initializing the GUI subsystem.", "Pekan");
            return false;
        }
        return true;
    }

    void GUISystem::exit()
    {
        exitImGui();
    }

    bool GUISystem::initImGui()
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

        // Setup Platform/Renderer2D backends
        ImGui_ImplGlfw_InitForOpenGL(PekanEngine::s_window.getGlfwWindow(), true);
        ImGui_ImplOpenGL3_Init(PK_GLSL_VERSION);

        // Scale up widgets and font by a factor of 2
        style.ScaleAllSizes(1.3f);
        io.FontGlobalScale = 1.05f;

        return true;
    }

    void GUISystem::exitImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace GUI
} // namespace Pekan
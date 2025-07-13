#include "GUI.h"

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

	static GUI g_gui;

	void GUI::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_gui);
	}

	GUI* GUI::getInstance()
	{
		return &g_gui;
	}

	void GUI::init()
	{
		m_isInitialized = initImGui();
		if (!m_isInitialized)
		{
			PK_LOG_ERROR("Failed to initialize ImGui when initializing the GUI subsystem.", "Pekan");
		}
	}

	void GUI::exit()
	{
		exitImGui();

		m_isInitialized = false;
	}

    bool GUI::initImGui()
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
        ImGui_ImplGlfw_InitForOpenGL(PekanEngine::s_window.getGlfwWindow(), true);
        ImGui_ImplOpenGL3_Init(PK_GLSL_VERSION);

        // Scale up widgets and font by a factor of 2
        style.ScaleAllSizes(1.3f);
        io.FontGlobalScale = 1.05f;

        return true;
    }

    void GUI::exitImGui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

} // namespace GUI
} // namespace Pekan
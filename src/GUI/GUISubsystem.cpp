#include "GUISubsystem.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"
#include "PekanEngine.h"
#include "PekanApplication.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#define PK_GLSL_VERSION "#version 330 core"

namespace Pekan
{
namespace GUI
{

	static GUISubsystem g_guiSystem;

	void GUISubsystem::registerAsSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_guiSystem);
	}

	GUISubsystem* GUISubsystem::getInstance()
	{
		return &g_guiSystem;
	}

	int GUISubsystem::generateWidgetId()
	{
		return m_nextWidgetId++;
	}

	bool GUISubsystem::init()
	{
		if (!initImGui())
		{
			PK_LOG_ERROR("Failed to initialize ImGui when initializing the GUI subsystem.", "Pekan");
			return false;
		}

		PekanApplication* application = PekanEngine::getApplication();
		if (application == nullptr)
		{
			PK_LOG_ERROR("Failed to get current application when initializing the GUI subsystem.", "Pekan");
			return false;
		}

		// Register beginFrame() and endFrame() to be automatically called at the beginning and end of each frame
		application->registerOnFrameBeginCallback([]() { GUISubsystem::beginFrame(); });
		application->registerOnFrameEndCallback([]() { GUISubsystem::endFrame(); });

		return true;
	}

	void GUISubsystem::exit()
	{
		exitImGui();
	}

	bool GUISubsystem::initImGui()
	{
		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;    // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;        // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      // Enable Multi-Viewport / Platform Windows

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

	void GUISubsystem::exitImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GUISubsystem::beginFrame()
	{
		if (m_isFrameActive)
		{
			PK_LOG_ERROR("Attempting to begin a new GUI frame while another frame is already active.", "Pekan");
			return;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_isFrameActive = true;
	}

	void GUISubsystem::endFrame()
	{
		if (!m_isFrameActive)
		{
			PK_LOG_ERROR("Attempting to end a GUI frame but no frame is currently active.", "Pekan");
			return;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it)
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		m_isFrameActive = false;
	}

} // namespace GUI
} // namespace Pekan
#include "GUIWindow.h"
#include "PekanEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

namespace Pekan
{
namespace GUI
{

    void GUIWindow::exit()
    {
        // Destroys all widgets of the GUI window
        for (const Widget_Ptr& widget : m_widgets)
        {
            widget->destroy();
        }
        // Call derived class' exit function
        _exit();
    }

	void GUIWindow::render() const
	{
        if (PekanEngine::getWindow().isMinimized())
        {
            ImGui_ImplGlfw_Sleep(10);
            return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Get GUI window's properties, potentially from derived class.
        // These properties will be the same for the whole lifetime of a GUI window
        // so we can keep them in a static variable here, to avoid calling getProperties() on every frame.
        static GUIWindowProperties properties = getProperties();

        ImGui::SetNextWindowSize(ImVec2(properties.size.x, properties.size.y));
        ImGui::Begin(properties.name.c_str());

        // Render all widgets of the GUI window
        for (const Widget_ConstPtr& widget : m_widgets)
        {
            widget->render();
        }
        // Call derived class' render function
        _render();

        ImGui::End();

        ImGui::Render();

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

    void GUIWindow::addWidget(const Widget_Ptr& widget)
    {
        m_widgets.push_back(widget);
    }

    bool GUIWindow::onKeyPressed(const KeyPressedEvent& event)
    {
        const ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureKeyboard;
    }

    bool GUIWindow::onKeyReleased(const KeyReleasedEvent& event)
    {
        const ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureKeyboard;
    }
    bool GUIWindow::onMouseMoved(const MouseMovedEvent& event)
    {
        const ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }

    bool GUIWindow::onMouseScrolled(const MouseScrolledEvent& event)
    {
        const ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }

    bool GUIWindow::onMouseButtonPressed(const MouseButtonPressedEvent& event)
    {
        const ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }

    bool GUIWindow::onMouseButtonReleased(const MouseButtonReleasedEvent& event)
    {
        const ImGuiIO& io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }

} // namespace GUI
} // namespace Pekan
#include "GUIWindow.h"
#include "PekanEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"

namespace Pekan
{
namespace GUI
{

	bool GUIWindow::init()
	{
		// Cache GUI window's properties
		m_properties = getProperties();

		return _init();
	}

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

		ImGui::SetNextWindowSize(ImVec2(m_properties.size.x, m_properties.size.y));
		ImGui::Begin(m_properties.name.c_str());

		// Render all widgets of the GUI window
		for (const Widget_ConstPtr& widget : m_widgets)
		{
			widget->render();
		}

		// Call derived class' render function
		_render();

		ImGui::End();
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
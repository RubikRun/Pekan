#include "FPSDisplayWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	// Number of times (approximately) per second that FPS will be updated in GUI
	static float FPS_UPDATES_PER_SECOND = 4.0f;

	void FPSDisplayWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);
	}
	void FPSDisplayWidget::destroy()
	{
		m_fpsCache = 0.0f;
		m_fpsWaitFrames = 0;
		Widget::destroy();
	}

	void FPSDisplayWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());

		// Update FPS if we have waited enough since the last FPS update
		m_fpsWaitFrames--;
		if (m_fpsWaitFrames < 1)
		{
			m_fpsCache = ImGui::GetIO().Framerate;
			m_fpsWaitFrames = int(m_fpsCache / FPS_UPDATES_PER_SECOND);
		}

		// Display current FPS
		ImGui::Text("%.3f ms/frame", 1000.0f / m_fpsCache);
		ImGui::Text("%.1f FPS", m_fpsCache);
	}

} // namespace GUI
} // namespace Pekan

#include "Widget.h"

#include "PekanLogger.h"
#include "GUI.h"

#include "GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void Widget::create(GUIWindow* guiWindow)
	{
		m_id = GUI::generateWidgetId();
		if (guiWindow != nullptr)
		{
			// Check if widget is owned by a shared_ptr.
			if (!weak_from_this().expired())
			{
				guiWindow->addWidget(shared_from_this());
			}
			// If it's not we can't add it to GUI window.
			else
			{
				PK_LOG_ERROR("Trying to create a widget in a GUI window, but your widget is NOT owned by a shared_ptr."
					" In order for this to work you must use shared_ptr for your widget.", "Pekan");
				PK_ASSERT_QUICK(false);
			}
		}
	}

	void Widget::destroy()
	{
		m_id = -1;
	}

	bool Widget::isValid() const
	{
		return m_id >= 0;
	}

	void Widget::render() const
	{
		PK_ASSERT_QUICK(m_id >= 0);

		ImGui::PushID(m_id);
		_render();
		ImGui::PopID();
	}

} // namespace GUI
} // namespace Pekan

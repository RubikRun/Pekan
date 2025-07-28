#include "Widget.h"

#include "PekanLogger.h"
#include "GUISystem.h"

#include "GUIWindow.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	void Widget::create(GUIWindow* guiWindow)
	{
		PK_ASSERT(m_id < 0, "Trying to create a Widget instance that is already created.", "Pekan");

		m_id = GUISystem::generateWidgetId();
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
		PK_ASSERT(m_id >= 0, "Trying to destroy a Widget instance that is not yet created.", "Pekan");
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

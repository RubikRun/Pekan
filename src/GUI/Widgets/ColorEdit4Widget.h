#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A widget allowing control over an RGBA color parameter in GUI.
	//
	// NOTE: Instances of this class MUST be owned by a ColorEdit4Widget_Ptr
	class ColorEdit4Widget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, glm::vec4 initialValue);
		void create(GUIWindow* guiWindow, const char* label);
		void create(GUIWindow* guiWindow, const char* label, glm::vec4 initialValue);
		void destroy();

		glm::vec4 getValue() const;

		const std::string& getLabel() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The 4 values making up the color
		mutable glm::vec4 m_value = { 1.0f, 1.0f, 1.0f, 1.0f };
	};

	typedef std::shared_ptr<ColorEdit4Widget> ColorEdit4Widget_Ptr;
	typedef std::shared_ptr<const ColorEdit4Widget> ColorEdit4Widget_ConstPtr;

} // namespace GUI
} // namespace Pekan

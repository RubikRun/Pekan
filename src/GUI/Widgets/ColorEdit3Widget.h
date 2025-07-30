#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A widget allowing control over an RGB color parameter in GUI.
	//
	// NOTE: Instances of this class MUST be owned by a ColorEdit3Widget_Ptr
	class ColorEdit3Widget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, glm::vec3 initialValue);
		void create(GUIWindow* guiWindow, const char* label);
		void create(GUIWindow* guiWindow, const char* label, glm::vec3 initialValue);
		void destroy();

		glm::vec3 getValue() const;

		const std::string& getLabel() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The 3 values making up the color
		mutable glm::vec3 m_value = { 1.0f, 1.0f, 1.0f };
	};

	typedef std::shared_ptr<ColorEdit3Widget> ColorEdit3Widget_Ptr;
	typedef std::shared_ptr<const ColorEdit3Widget> ColorEdit3Widget_ConstPtr;

} // namespace GUI
} // namespace Pekan

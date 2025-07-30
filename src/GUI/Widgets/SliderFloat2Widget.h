#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A widget allowing control over 2 floating point number parameters in GUI.
	//
	// NOTE: Instances of this class MUST be owned by a SliderFloat2Widget_Ptr
	class SliderFloat2Widget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, glm::vec2 initialValue, float min = 0.0f, float max = 1.0f, const std::string& format = "%.3f");
		void create(GUIWindow* guiWindow, const char* label, float min = 0.0f, float max = 1.0f, const std::string& format = "%.3f");
		void create(GUIWindow* guiWindow, const char* label, glm::vec2 initialValue, float min = 0.0f, float max = 1.0f, const std::string& format = "%.3f");
		void destroy();

		glm::vec2 getValue() const;

		const std::string& getLabel() const;

		float getMin() const;
		float getMax() const;
		const std::string& getFormat() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The 2 float values of the widget
		mutable glm::vec2 m_value = { 0.0f, 0.0f };

		// Minimum and maximum values allowed for the 2 sliders
		//
		// NOTE: These minimum and maximum values apply only for sliding.
		//       Manually, you can enter any value.
		float m_min = 0.0f;
		float m_max = 1.0f;

		// Formatting string specifying how to format
		// the 2 floating point numbers when displaying them next to the slider.
		// (It's a classic C style formatting string)
		std::string m_format = "%.3f";
	};

	typedef std::shared_ptr<SliderFloat2Widget> SliderFloat2Widget_Ptr;
	typedef std::shared_ptr<const SliderFloat2Widget> SliderFloat2Widget_ConstPtr;

} // namespace GUI
} // namespace Pekan

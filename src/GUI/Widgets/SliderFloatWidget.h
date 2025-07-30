#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A widget allowing control over a floating point number parameter in GUI.
	//
	// NOTE: Instances of this class MUST be owned by a SliderFloatWidget_Ptr
	class SliderFloatWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, float initialValue, float min = 0.0f, float max = 1.0f, const std::string& format = "%.3f");
		void create(GUIWindow* guiWindow, const char* label, float min = 0.0f, float max = 1.0f, const std::string& format = "%.3f");
		void create(GUIWindow* guiWindow, const char* label, float initialValue, float min = 0.0f, float max = 1.0f, const std::string& format = "%.3f");
		void destroy();

		float getValue() const;

		const std::string& getLabel() const;

		float getMin() const;
		float getMax() const;
		const std::string& getFormat() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The float value of the widget
		mutable float m_value = 0.0f;

		// Minimum and maximum values allowed for the slider
		//
		// NOTE: These minimum and maximum values apply only for sliding.
		//       Manually, you can enter any value.
		float m_min = 0.0f;
		float m_max = 1.0f;

		// Formatting string specifying how to format
		// the floating point number when displaying it next to the slider.
		// (It's a classic C style formatting string)
		std::string m_format = "%.3f";
	};

	typedef std::shared_ptr<SliderFloatWidget> SliderFloatWidget_Ptr;
	typedef std::shared_ptr<const SliderFloatWidget> SliderFloatWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

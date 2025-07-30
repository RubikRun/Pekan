#pragma once

#include "Widget.h"

#include <string>
#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	// A widget allowing control over an integer parameter in GUI.
	//
	// NOTE: Instances of this class MUST be owned by a SliderIntWidget_Ptr
	class SliderIntWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, int initialValue, int min = 1, int max = 10);
		void create(GUIWindow* guiWindow, const char* label, int min = 1, int max = 10);
		void create(GUIWindow* guiWindow, const char* label, int initialValue, int min = 1, int max = 10);
		void destroy();

		int getValue() const;

		const std::string& getLabel() const;

		int getMin() const;
		int getMax() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Label to be displayed next to the widget
		std::string m_label;

		// The integer value of the widget
		mutable int m_value = 0;

		// Minimum and maximum values allowed for the slider
		//
		// NOTE: These minimum and maximum values apply only for sliding.
		//       Manually, you can enter any value.
		int m_min = 1;
		int m_max = 10;
	};

	typedef std::shared_ptr<SliderIntWidget> SliderIntWidget_Ptr;
	typedef std::shared_ptr<const SliderIntWidget> SliderIntWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

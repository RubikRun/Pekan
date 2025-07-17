#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	class SliderIntWidget
	{
	public:

		SliderIntWidget();
		SliderIntWidget(int initialValue, int min = 1, int max = 10);
		SliderIntWidget(const char* label, int min = 1, int max = 10);
		SliderIntWidget(const char* label, int initialValue, int min = 1, int max = 10);

		void render() const;

		inline int getValue() const { return m_value; }

	private:

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

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

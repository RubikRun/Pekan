#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	class ColorEdit4Widget
	{
	public:

		ColorEdit4Widget();
		ColorEdit4Widget(glm::vec4 initialValue);
		ColorEdit4Widget(const char* label);
		ColorEdit4Widget(const char* label, glm::vec4 initialValue);

		void render() const;

		inline glm::vec4 getValue() const { return m_value; }

	private:

		// Label to be displayed next to the widget
		std::string m_label;

		// The 4 values making up the color
		mutable glm::vec4 m_value = { 1.0f, 1.0f, 1.0f, 1.0f };

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

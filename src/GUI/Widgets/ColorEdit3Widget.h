#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	class ColorEdit3Widget
	{
	public:

		ColorEdit3Widget();
		ColorEdit3Widget(glm::vec3 initialValue);
		ColorEdit3Widget(const char* label);
		ColorEdit3Widget(const char* label, glm::vec3 initialValue);

		void render() const;

		inline glm::vec3 getValue() const { return m_value; }

	private:

		// Label to be displayed next to the widget
		std::string m_label;

		// The 3 values making up the color
		mutable glm::vec3 m_value = { 1.0f, 1.0f, 1.0f };

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

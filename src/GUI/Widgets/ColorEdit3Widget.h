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

		ColorEdit3Widget() = default;
		ColorEdit3Widget(glm::vec3 initialValue);
		ColorEdit3Widget(const char* label);
		ColorEdit3Widget(const char* label, glm::vec3 initialValue);

		void render();

		inline glm::vec3 getValue() const { return m_value; }

	private:

		// Label to be displayed next to the widget
		std::string m_label;

		// The 3 color values of the widget
		glm::vec3 m_value = { 0.0f, 0.0f, 0.0f };
	};

} // namespace GUI
} // namespace Pekan

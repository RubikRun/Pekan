#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	class DragFloat2Widget
	{
	public:

		DragFloat2Widget();
		DragFloat2Widget(glm::vec2 initialValue, float min = 0.0f, float max = 1.0f, float step = 0.01f);
		DragFloat2Widget(const char* label, float min = 0.0f, float max = 1.0f, float step = 0.01f);
		DragFloat2Widget(const char* label, glm::vec2 initialValue, float min = 0.0f, float max = 1.0f, float step = 0.01f);

		void render() const;

		inline glm::vec2 getValue() const { return m_value; }

	private:

		// Label to be displayed next to the widget
		std::string m_label;

		// The 2 values of the widget
		mutable glm::vec2 m_value = { 0.0f, 0.0f };

		// Minimum and maximum values allowed for the 2 drags.
		//
		// NOTE: These minimum and maximum values apply only for dragging.
		//       Manually, you can enter any value.
		float m_min = 0.0f;
		float m_max = 1.0f;

		// Incremental step used for the 2 drags.
		// This is the smallest change that you can do to a value by dragging.
		// It determines how precisely you can drag.
		float m_step = 0.01f;

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	class ButtonWidget
	{
	public:

		ButtonWidget();
		ButtonWidget(const char* label);

		void render() const;

		inline bool isClicked() const { return m_isClicked; }

	private:

		// Label to be displayed next to the widget
		std::string m_label;

		// The "is currently clicked" state of the button
		mutable bool m_isClicked = false;

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

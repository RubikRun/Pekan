#pragma once

#include <string>

namespace Pekan
{
namespace GUI
{

	class TextWidget
	{
	public:

		// Creates a text widget with empty text
		TextWidget();
		// Creates a text widget with a given text
		TextWidget(const char* text);

		void render() const;

		inline const std::string& getText() const { return m_text; }

	private:

		// Text to be displayed on the text widget
		std::string m_text;

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

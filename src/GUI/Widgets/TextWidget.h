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
		TextWidget() = default;
		// Creates a text widget with a given text
		TextWidget(const char* text);

		void render();

	private:

		// Text to be displayed on the text widget
		std::string m_text;
	};

} // namespace GUI
} // namespace Pekan

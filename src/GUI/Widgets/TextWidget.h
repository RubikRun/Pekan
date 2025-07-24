#pragma once

#include "Widget.h"

#include <string>

namespace Pekan
{
namespace GUI
{

	// A widget displaying some text.
	//
	// NOTE: Instances of this class MUST be owned by a TextWidget_Ptr
	class TextWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void create(GUIWindow* guiWindow, const char* text);
		void destroy();

		const std::string& getText() const;

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// Text to be displayed on the text widget
		std::string m_text;
	};

	typedef std::shared_ptr<TextWidget> TextWidget_Ptr;
	typedef std::shared_ptr<const TextWidget> TextWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

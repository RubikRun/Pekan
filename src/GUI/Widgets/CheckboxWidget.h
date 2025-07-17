#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Pekan
{
namespace GUI
{

	class CheckboxWidget
	{
	public:

		CheckboxWidget();
		CheckboxWidget(bool initialIsChecked);
		CheckboxWidget(const char* label);
		CheckboxWidget(const char* label, bool initialIsChecked);

		void render() const;

		inline bool isChecked() const { return m_isChecked; }

	private:

		// Label to be displayed next to the widget
		std::string m_label;

		// The checked states of the checkbox
		mutable bool m_isChecked = false;

		// Widget's ID in the GUI subsystem
		int m_id = -1;
	};

} // namespace GUI
} // namespace Pekan

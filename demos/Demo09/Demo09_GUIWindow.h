#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"
#include "ComboBoxWidget.h"

namespace Demo
{

	class Demo09_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo09_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		// Returns index of selected post-processing shader
		int getPpsIndex() const { return gui.ppsComboBoxWidget->getIndex(); }

	private: /* functions */

		bool _init() override;

		Pekan::GUI::GUIWindowProperties getProperties() const override;

		struct Widgets
		{
			Pekan::GUI::ComboBoxWidget_Ptr ppsComboBoxWidget = std::make_shared<Pekan::GUI::ComboBoxWidget>();
			Pekan::GUI::TextWidget_Ptr ppsLabel = std::make_shared<Pekan::GUI::TextWidget>();
		} gui;
	};

} // namespace Demo


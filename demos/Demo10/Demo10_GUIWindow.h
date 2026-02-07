#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "ColorEdit3Widget.h"
#include "DragFloat2Widget.h"
#include "ComboBoxWidget.h"
#include "SeparatorWidget.h"
#include "SliderFloat2Widget.h"
#include "SliderFloatWidget.h"

namespace Demo
{

	class Demo10_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

	Demo10_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		glm::vec2    getBodyPosition() const     { return gui.bodyWidgets.positionWidget->getValue(); }
		float        getBodyRotation() const     { return gui.bodyWidgets.rotationWidget->getValue(); }
		glm::vec2    getBodyScale() const        { return gui.bodyWidgets.scaleWidget->getValue(); }

		glm::vec2    getLeftArmPosition() const    { return gui.leftArmWidgets.positionWidget->getValue(); }
		float        getLeftArmRotation() const    { return gui.leftArmWidgets.rotationWidget->getValue(); }
		glm::vec2    getLeftArmScale() const       { return gui.leftArmWidgets.scaleWidget->getValue(); }

		glm::vec2    getRightArmPosition() const   { return gui.rightArmWidgets.positionWidget->getValue(); }
		float        getRightArmRotation() const   { return gui.rightArmWidgets.rotationWidget->getValue(); }
		glm::vec2    getRightArmScale() const      { return gui.rightArmWidgets.scaleWidget->getValue(); }

		glm::vec2    getLeftLegPosition() const    { return gui.leftLegWidgets.positionWidget->getValue(); }
		float        getLeftLegRotation() const    { return gui.leftLegWidgets.rotationWidget->getValue(); }
		glm::vec2    getLeftLegScale() const       { return gui.leftLegWidgets.scaleWidget->getValue(); }

		glm::vec2    getRightLegPosition() const   { return gui.rightLegWidgets.positionWidget->getValue(); }
		float        getRightLegRotation() const   { return gui.rightLegWidgets.rotationWidget->getValue(); }
		glm::vec2    getRightLegScale() const      { return gui.rightLegWidgets.scaleWidget->getValue(); }

		glm::vec2    getHeadPosition() const    { return gui.headWidgets.positionWidget->getValue(); }
		float        getHeadRotation() const    { return gui.headWidgets.rotationWidget->getValue(); }
		glm::vec2    getHeadScale() const       { return gui.headWidgets.scaleWidget->getValue(); }

		glm::vec2    getSwordPosition() const    { return gui.swordWidgets.positionWidget->getValue(); }
		float        getSwordRotation() const    { return gui.swordWidgets.rotationWidget->getValue(); }
		glm::vec2    getSwordScale() const       { return gui.swordWidgets.scaleWidget->getValue(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			struct BodyWidgets
			{
				Pekan::GUI::TextWidget_Ptr label =                     std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator =            std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget =    std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget =       std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} bodyWidgets;
			struct LeftArmWidgets
			{
				Pekan::GUI::TextWidget_Ptr label =                     std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator =            std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget =    std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget =       std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} leftArmWidgets;
			struct RightArmWidgets
			{
				Pekan::GUI::TextWidget_Ptr label =                     std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator =            std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget =    std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget =       std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} rightArmWidgets;
			struct LeftLegWidgets
			{
				Pekan::GUI::TextWidget_Ptr label =                     std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator =            std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget =    std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget =       std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} leftLegWidgets;
			struct RightLegWidgets
			{
				Pekan::GUI::TextWidget_Ptr label =                     std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator =            std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget =    std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget =       std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} rightLegWidgets;
			struct HeadWidgets
			{
				Pekan::GUI::TextWidget_Ptr label = std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator = std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget = std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget = std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget = std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} headWidgets;
			struct SwordWidgets
			{
				Pekan::GUI::TextWidget_Ptr label = std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator = std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget = std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget = std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget = std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} swordWidgets;
		} gui;
	};

} // namespace Demo
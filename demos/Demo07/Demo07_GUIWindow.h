#pragma once

#include "GUIWindow.h"

#include "TextWidget.h"
#include "SeparatorWidget.h"
#include "SliderFloat2Widget.h"
#include "SliderFloatWidget.h"
#include "NewLineWidget.h"

namespace Demo
{

	class Demo07_GUIWindow : public Pekan::GUI::GUIWindow
	{
	public:

		Demo07_GUIWindow(Pekan::PekanApplication* application) : GUIWindow(application) {}

		bool init() override;

		glm::vec2 getRectanglePosition() const { return gui.shapeTypeWidgets[0].positionWidget->getValue(); }
		float getRectangleRotation() const { return gui.shapeTypeWidgets[0].rotationWidget->getValue(); }
		glm::vec2 getRectangleScale() const { return gui.shapeTypeWidgets[0].scaleWidget->getValue(); }

		glm::vec2 getCirclePosition() const { return gui.shapeTypeWidgets[1].positionWidget->getValue(); }
		float getCircleRotation() const { return gui.shapeTypeWidgets[1].rotationWidget->getValue(); }
		glm::vec2 getCircleScale() const { return gui.shapeTypeWidgets[1].scaleWidget->getValue(); }

		glm::vec2 getTrianglePosition() const { return gui.shapeTypeWidgets[2].positionWidget->getValue(); }
		float getTriangleRotation() const { return gui.shapeTypeWidgets[2].rotationWidget->getValue(); }
		glm::vec2 getTriangleScale() const { return gui.shapeTypeWidgets[2].scaleWidget->getValue(); }

		glm::vec2 getPolygonPosition() const { return gui.shapeTypeWidgets[3].positionWidget->getValue(); }
		float getPolygonRotation() const { return gui.shapeTypeWidgets[3].rotationWidget->getValue(); }
		glm::vec2 getPolygonScale() const { return gui.shapeTypeWidgets[3].scaleWidget->getValue(); }

		glm::vec2 getLinePosition() const { return gui.shapeTypeWidgets[4].positionWidget->getValue(); }
		float getLineRotation() const { return gui.shapeTypeWidgets[4].rotationWidget->getValue(); }
		glm::vec2 getLineScale() const { return gui.shapeTypeWidgets[4].scaleWidget->getValue(); }

	private: /* functions */

		Pekan::GUI::GUIWindowProperties getProperties() const override;

	private: /* variables */

		struct Widgets
		{
			// Widgets for a single shape type
			struct ShapeTypeWidgets
			{
				Pekan::GUI::TextWidget_Ptr label =                     std::make_shared<Pekan::GUI::TextWidget>();
				Pekan::GUI::SeparatorWidget_Ptr separator =            std::make_shared<Pekan::GUI::SeparatorWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr positionWidget =    std::make_shared<Pekan::GUI::SliderFloat2Widget>();
				Pekan::GUI::SliderFloatWidget_Ptr rotationWidget =     std::make_shared<Pekan::GUI::SliderFloatWidget>();
				Pekan::GUI::SliderFloat2Widget_Ptr scaleWidget =       std::make_shared<Pekan::GUI::SliderFloat2Widget>();
			} shapeTypeWidgets[5];

			Pekan::GUI::NewLineWidget_Ptr newLineWidgets[4] =
			{
				std::make_shared<Pekan::GUI::NewLineWidget>(),
				std::make_shared<Pekan::GUI::NewLineWidget>(),
				std::make_shared<Pekan::GUI::NewLineWidget>(),
				std::make_shared<Pekan::GUI::NewLineWidget>(),
			};
		} gui;
	};

} // namespace Demo
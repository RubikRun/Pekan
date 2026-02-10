#include "Demo10_GUIWindow.h"

#include "PekanLogger.h"
#include "RenderCommands.h"

#define PI 3.14159265359f

using namespace Pekan;
using namespace Pekan::GUI;

namespace Demo
{

	bool Demo10_GUIWindow::_init()
	{
		// Create widgets for player body transform
		gui.bodyWidgets.label->create(this, "Body");
		gui.bodyWidgets.separator->create(this);
		gui.bodyWidgets.positionWidget->create(this, "Position", { 0.0f, 0.0f }, -10.0f, 10.0f);
		gui.bodyWidgets.rotationWidget->create(this, "Rotation", 0.0f, 0.0f, PI * 2.0f);
		gui.bodyWidgets.scaleWidget->create(this, "Scale", { 1.0f, 1.0f }, -3.0f, 3.0f);
		// Create widgets for player left arm transform
		gui.leftArmWidgets.label->create(this, "Left Arm");
		gui.leftArmWidgets.separator->create(this);
		gui.leftArmWidgets.positionWidget->create(this, "Position", { -1.43f, 0.2f }, -10.0f, 10.0f);
		gui.leftArmWidgets.rotationWidget->create(this, "Rotation", 0.0f, -PI, PI * 2.0f);
		gui.leftArmWidgets.scaleWidget->create(this, "Scale", { 1.0f, 1.0f }, -3.0f, 3.0f);
		// Create widgets for player right arm transform
		gui.rightArmWidgets.label->create(this, "Right Arm");
		gui.rightArmWidgets.separator->create(this);
		gui.rightArmWidgets.positionWidget->create(this, "Position", { 1.43f, 0.2f }, -10.0f, 10.0f);
		gui.rightArmWidgets.rotationWidget->create(this, "Rotation", 0.0f, -PI, PI * 2.0f);
		gui.rightArmWidgets.scaleWidget->create(this, "Scale", { -1.0f, 1.0f }, -3.0f, 3.0f);
		// Create widgets for player left leg transform
		gui.leftLegWidgets.label->create(this, "Left Leg");
		gui.leftLegWidgets.separator->create(this);
		gui.leftLegWidgets.positionWidget->create(this, "Position", { -1.0f, -1.33f }, -10.0f, 10.0f);
		gui.leftLegWidgets.rotationWidget->create(this, "Rotation", 0.0f, -PI, PI * 2.0f);
		gui.leftLegWidgets.scaleWidget->create(this, "Scale", { 3.0f, 3.0f }, -5.0f, 5.0f);
		// Create widgets for player right leg transform
		gui.rightLegWidgets.label->create(this, "Right Leg");
		gui.rightLegWidgets.separator->create(this);
		gui.rightLegWidgets.positionWidget->create(this, "Position", { 1.0f, -1.33f }, -10.0f, 10.0f);
		gui.rightLegWidgets.rotationWidget->create(this, "Rotation", 0.0f, -PI, PI * 2.0f);
		gui.rightLegWidgets.scaleWidget->create(this, "Scale", { -3.0f, 3.0f }, -5.0f, 5.0f);
		// Create widgets for player head transform
		gui.headWidgets.label->create(this, "Head");
		gui.headWidgets.separator->create(this);
		gui.headWidgets.positionWidget->create(this, "Position", { 0.0f, 2.891f }, -10.0f, 10.0f);
		gui.headWidgets.rotationWidget->create(this, "Rotation", 0.0f, 0.0f, PI * 2.0f);
		gui.headWidgets.scaleWidget->create(this, "Scale", { 2.0f, 2.0f }, -5.0f, 5.0f);
		// Create widgets for player sword transform
		gui.swordWidgets.label->create(this, "Sword");
		gui.swordWidgets.separator->create(this);
		gui.swordWidgets.positionWidget->create(this, "Position", { -1.0f, 1.468f }, -10.0f, 10.0f);
		gui.swordWidgets.rotationWidget->create(this, "Rotation", 0.665f, -PI, PI * 2.0f);
		gui.swordWidgets.scaleWidget->create(this, "Scale", { 2.5f, 2.5f }, -3.0f, 3.0f);

		return true;
	}

	GUIWindowProperties Demo10_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 400, 700 };
		props.name = "Demo10";
		return props;
	}

} // namespace Demo

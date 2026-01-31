#include "Demo06_GUIWindow.h"

#include "PekanLogger.h"

using namespace Pekan::GUI;

namespace Demo
{

	bool Demo06_GUIWindow::init()
	{
		gui.numberOfShapesLabel->create(this, "Number Of Shapes");
		gui.numberOfShapesWidget->create(this, "", 100, 5, 600);

		gui.showRectanglesWidget->create(this, "Show Rectangles", true);
		gui.showCirclesWidget->create(this, "Show Circles", true);
		gui.showTrianglesWidget->create(this, "Show Triangles", true);
		gui.showPolygonsWidget->create(this, "Show Polygons", true);
		gui.showLinesWidget->create(this, "Show Lines", true);
		gui.ppsLabel->create(this, "Post-Processing Shader");
		gui.ppsComboBoxWidget->create(this, { "Identity", "Sharpen", "Blur", "Edge Detection", "Emboss" });
		gui.fpsDisplayWidget->create(this);

		return true;
	}

	GUIWindowProperties Demo06_GUIWindow::getProperties() const
	{
		GUIWindowProperties props;
		props.size = { 300, 420 };
		props.name = "Demo06";
		return props;
	}

} // namespace Demo
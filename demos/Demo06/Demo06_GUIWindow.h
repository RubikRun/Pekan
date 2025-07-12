#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"

namespace Demo
{

	class Demo06_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo06_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		// Returns current number of shapes
		inline int getNumberOfShapes() const { return m_numberOfShapes; }

		inline bool isEnabledRectangles() const { return m_isEnabledRectangles; }
		inline bool isEnabledCircles() const { return m_isEnabledCircles; }
		inline bool isEnabledCirclesStatic() const { return m_isEnabledCirclesStatic; }
		inline bool isEnabledTriangles() const { return m_isEnabledTriangles; }
		inline bool isEnabledPolygons() const { return m_isEnabledPolygons; }
		inline bool isEnabledLines() const { return m_isEnabledLines; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		int m_numberOfShapes = 60000;

		bool m_isEnabledRectangles = true;
		bool m_isEnabledCircles = true;
		bool m_isEnabledCirclesStatic = true;
		bool m_isEnabledTriangles = true;
		bool m_isEnabledPolygons = true;
		bool m_isEnabledLines = true;

		// We'll cache the FPS here from time to time,
		// because I don't want to update the FPS in GUI every frame - it's flickering annoyingly.
		float m_fpsCache = 0.0f;
		// Number of frames to wait until the next FPS update
		int m_fpsWaitFrames = 0;
	};

} // namespace Demo
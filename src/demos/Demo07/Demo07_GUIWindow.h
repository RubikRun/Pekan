#pragma once

#include "PekanGUIWindow.h"
using Pekan::PekanGUIWindow;

#include "imgui.h"
#include <glm/glm.hpp>

namespace Demo
{

	class Demo07_GUIWindow : public PekanGUIWindow
	{
	public:

		Demo07_GUIWindow(Pekan::PekanApplication* application) : PekanGUIWindow(application) {}

		inline glm::vec2 getRectanglePosition() const { return m_rectanglePosition; }
		inline float getRectangleRotation() const { return m_rectangleRotation; }
		inline glm::vec2 getRectangleScale() const { return m_rectangleScale; }

		inline glm::vec2 getCirclePosition() const { return m_circlePosition; }
		inline float getCircleRotation() const { return m_circleRotation; }
		inline glm::vec2 getCircleScale() const { return m_circleScale; }

		inline glm::vec2 getCircleStaticPosition() const { return m_circleStaticPosition; }
		inline float getCircleStaticRotation() const { return m_circleStaticRotation; }
		inline glm::vec2 getCircleStaticScale() const { return m_circleStaticScale; }

		inline glm::vec2 getTrianglePosition() const { return m_trianglePosition; }
		inline float getTriangleRotation() const { return m_triangleRotation; }
		inline glm::vec2 getTriangleScale() const { return m_triangleScale; }

		inline glm::vec2 getPolygonPosition() const { return m_polygonPosition; }
		inline float getPolygonRotation() const { return m_polygonRotation; }
		inline glm::vec2 getPolygonScale() const { return m_polygonScale; }

		inline glm::vec2 getLinePosition() const { return m_linePosition; }
		inline float getLineRotation() const { return m_lineRotation; }
		inline glm::vec2 getLineScale() const { return m_lineScale; }

	private: /* functions */

		void _render() override;

		bool init() override;

	private: /* variables */

		glm::vec2 m_rectanglePosition = glm::vec2(1.5f, 1.0f);
		float m_rectangleRotation = 0.0f;
		glm::vec2 m_rectangleScale = glm::vec2(1.0f, 1.0f);

		glm::vec2 m_circlePosition = glm::vec2(-1.5f, 1.0f);
		float m_circleRotation = 0.0f;
		glm::vec2 m_circleScale = glm::vec2(1.0f, 1.0f);

		glm::vec2 m_circleStaticPosition = glm::vec2(0.0f, 0.0f);
		float m_circleStaticRotation = 0.0f;
		glm::vec2 m_circleStaticScale = glm::vec2(1.0f, 1.0f);

		glm::vec2 m_trianglePosition = glm::vec2(1.5f, -1.0f);
		float m_triangleRotation = 0.0f;
		glm::vec2 m_triangleScale = glm::vec2(1.0f, 1.0f);

		glm::vec2 m_polygonPosition = glm::vec2(-1.5f, -2.0f);
		float m_polygonRotation = 0.0f;
		glm::vec2 m_polygonScale = glm::vec2(1.0f, 1.0f);

		glm::vec2 m_linePosition = glm::vec2(-1.5f, -2.0f);
		float m_lineRotation = 0.0f;
		glm::vec2 m_lineScale = glm::vec2(1.0f, 1.0f);
	};

} // namespace Demo
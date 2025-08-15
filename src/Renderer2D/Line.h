#pragma once

#include "RenderObject.h"
#include "Camera2D.h"

#include <glm/glm.hpp>

namespace Pekan
{
namespace Renderer2D
{

	// A class representing a 2D line.
	//
	// NOTE: This class is used primarily for debugging and development-time graphics.
	//       It renders a line primitive very directly, without any optimization.
	//       For an actual game/application it's better to use
	//           class LineShape
	//       It is better optimized and allows control of line's thickness.
	//
	// NOTE: Another difference between Line and LineShape is that a Line always has a thickness of 1 pixel,
	//       no matter how much you zoom in/out with a camera,
	//       while LineShape has a world-space thickness so if you zoom in it will appear thicker
	//       and if you zoom out it will appear thinner.
	class Line
	{
	public:

		// Creates a line between 2 points
		void create(glm::vec2 pointA, glm::vec2 pointB);
		void destroy();

		void update();
		void render() const;

		void setPointA(glm::vec2 pointA);
		void setPointB(glm::vec2 pointB);

		inline glm::vec2 getPointA() const { return m_vertices[0]; }
		inline glm::vec2 getPointB() const { return m_vertices[1]; }

		inline glm::vec4 getColor() const { return m_color; }
		void setColor(glm::vec4 color);

	private: /* variables */

		Graphics::RenderObject m_renderObject;

		// The 2 vertices of the line, in world space
		glm::vec2 m_vertices[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f) };

		glm::vec4 m_color;
	};

} // namespace Renderer2D
} // namespace Pekan

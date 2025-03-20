#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Demo02_GUIWindow.h"

namespace Demo
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	class Demo02_Scene : public Pekan::PekanScene
	{
	public:

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

		// Returns a (const) reference to the list of colors
		const std::vector<glm::vec3>& getColors() const { return m_colors; }
		std::vector<glm::vec3>& getColors() { return m_colors; }

	private: /* variables */

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::Shader m_shader;

		// List holding current colors of cube's sides
		std::vector<glm::vec3> m_colors;
		// Cube's vertices
		std::vector<Vertex> m_vertices;

		// Current rotation of the cube, in radians
		float m_rotation = 0.0f;

		// Transform matrices
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;
	};

} // namespace Demo
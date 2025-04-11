#pragma once

#include "Layer.h"
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

	class Demo02_Scene : public Pekan::Layer
	{
	public:

		Demo02_Scene() : Layer("scene_layer") {}

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

		// Returns a (const) reference to the list of colors
		inline const std::vector<glm::vec3>& getColors() const { return m_colors; }
		inline std::vector<glm::vec3>& getColors() { return m_colors; }

		// Returns (a reference to) cube's rotation
		inline float getRotation() const { return m_rotation; }
		inline float& getRotation() { return m_rotation; }

		// Returns (a reference to) camera's FOV
		inline float getFOV() const { return m_fov; }
		inline float& getFOV() { return m_fov; }

		// Returns (a reference to) camera's distance to scene's center
		inline float getCameraDist() const { return m_cameraDist; }
		inline float& getCameraDist() { return m_cameraDist; }

	private: /* variables */

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::Shader m_shader;

		// List holding current colors of cube's sides
		std::vector<glm::vec3> m_colors;
		// Cube's vertices
		std::vector<Vertex> m_vertices;

		// Current rotation of the cube, in degrees
		float m_rotation = 0.0f;

		// Field of view (vertical), in degrees
		float m_fov = 30.0f;

		// Distance from camera to scene's center
		float m_cameraDist = 2.5f;

		// Transform matrices
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;

		glm::ivec2 m_resolution;
	};

} // namespace Demo
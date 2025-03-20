#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "Demo02_GUIWindow.h"

namespace Demo
{

	class Demo02_Scene : public Pekan::PekanScene
	{
	public:

		bool init() override;

		void update() override;

		void render() override;

		void exit() override;

	private: /* variables */

		Pekan::Renderer::VertexBuffer m_vertexBuffer;

		Pekan::Renderer::VertexArray m_vertexArray;

		Pekan::Renderer::IndexBuffer m_indexBuffer;

		Pekan::Renderer::Shader m_shader;

		// Current rotation of the cube, in radians
		float m_rotation = 0.0f;

		// Transform matrices
		glm::mat4 m_modelMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;
	};

} // namespace Demo
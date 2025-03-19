#pragma once

#include "PekanScene.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
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
	};

} // namespace Demo
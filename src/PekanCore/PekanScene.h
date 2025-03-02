#pragma once

namespace Pekan
{

	// A base class for all Pekan scenes
	class PekanScene
	{
	public:

		virtual ~PekanScene() = default;

		virtual bool init() = 0;

		virtual void update() = 0;

		virtual void render() = 0;

		virtual void exit() = 0;
	};

} // namespace Pekan
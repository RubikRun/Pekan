#pragma once

namespace Pekan
{

	// A base class for all Pekan scenes
	class PekanScene
	{
	public:

		virtual ~PekanScene() = default;

		bool init(int width, int height)
		{
			this->width = width;
			this->height = height;
			return _init();
		}

		virtual void update() = 0;

		virtual void render() = 0;

		virtual void exit() = 0;

	private: /* functions */

		virtual bool _init() = 0;

	protected: /* variables */

		// Width of scene, in pixels
		int width = -1;
		// Height of scene, in pixels
		int height = -1;
	};

} // namespace Pekan
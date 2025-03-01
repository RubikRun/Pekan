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
			m_width = width;
			m_height = height;
			return _init();
		}

		virtual void update() = 0;

		virtual void render() = 0;

		virtual void exit() = 0;

	private: /* functions */

		virtual bool _init() = 0;

	protected: /* variables */

		// Width of scene, in pixels
		int m_width = -1;
		// Height of scene, in pixels
		int m_height = -1;
	};

} // namespace Pekan
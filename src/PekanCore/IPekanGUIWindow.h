#pragma once

struct GLFWwindow;

namespace Pekan
{

	// A base class for all Pekan GUI windows
	class IPekanGUIWindow
	{
	public:

		virtual ~IPekanGUIWindow() = default;

		virtual bool init() = 0;

		void render();

	private: /* functions*/

		virtual void _render() = 0;
	};

} // namespace Pekan
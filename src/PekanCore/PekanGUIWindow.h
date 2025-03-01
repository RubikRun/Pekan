#pragma once

struct GLFWwindow;

namespace Pekan
{

	// A base class for all Pekan GUI windows
	class PekanGUIWindow
	{
	public:

		virtual ~PekanGUIWindow() = default;

		bool init(GLFWwindow* window)
		{
			m_window = window;
			return _init();
		}

		void render();

	private: /* functions*/

		virtual void _render() = 0;

		virtual bool _init() = 0;

	private: /* variables */

		// Graphics window where the GUI window is rendered
		GLFWwindow* m_window = nullptr;
	};

} // namespace Pekan
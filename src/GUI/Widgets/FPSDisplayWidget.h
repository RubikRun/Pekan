#pragma once

#include "Widget.h"

#include <string>

namespace Pekan
{
namespace GUI
{

	// A widget displaying FPS (frame per second) at every moment
	//
	// NOTE: Instances of this class MUST be owned by a FPSDisplayWidget_Ptr
	class FPSDisplayWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void destroy();

	private: /* functions */

		void _render() const override;

	private: /* variables */

		// We'll cache the FPS here from time to time,
		// so that we don't update the FPS in GUI every frame.
		mutable float m_fpsCache = 0.0f;
		// Number of frames to wait until the next FPS update
		mutable int m_fpsWaitFrames = 0;
	};

	typedef std::shared_ptr<FPSDisplayWidget> FPSDisplayWidget_Ptr;
	typedef std::shared_ptr<const FPSDisplayWidget> FPSDisplayWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

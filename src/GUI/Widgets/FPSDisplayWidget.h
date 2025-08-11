#pragma once

#include "Widget.h"

#include <deque>
#include <string>

namespace Pekan
{
namespace GUI
{

	// A widget displaying FPS (frames per second) in real-time and over different time windows
	//
	// NOTE: Instances of this class MUST be owned by a FPSDisplayWidget_Ptr
	class FPSDisplayWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void destroy();

	private: /* functions */

		void _render() const override;

		// Calculates average FPS over the last N seconds
		float getAverageFpsOverSeconds(float seconds) const;

	private: /* variables */

		// We'll cache the FPS here from time to time,
		// so that we don't update the FPS in GUI every frame.
		mutable float m_fpsCache = 0.0f;
		// Number of frames to wait until the next FPS update
		mutable int m_fpsWaitFramesTilUpdate = 0;

		// Frame history for calculating average FPS over different periods
		mutable std::deque<float> m_fpsHistory;
		// Total accumulated FPS for "since start" average
		mutable double m_totalFpsSum = 0.0;
		mutable size_t m_totalFpsCount = 0ULL;
		mutable float m_totalFps = 0.0f;
		// FPS over the last 5s, 10s and 20s
		mutable float m_fps5s = 0.0f;
		mutable float m_fps10s = 0.0f;
		mutable float m_fps20s = 0.0f;
	};

	typedef std::shared_ptr<FPSDisplayWidget> FPSDisplayWidget_Ptr;
	typedef std::shared_ptr<const FPSDisplayWidget> FPSDisplayWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

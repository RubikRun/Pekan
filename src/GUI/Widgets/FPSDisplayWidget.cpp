#include "FPSDisplayWidget.h"

#include "PekanLogger.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Pekan
{
namespace GUI
{

	// Number of times (approximately) per second that FPS will be updated in GUI
	static constexpr float FPS_UPDATES_PER_SECOND = 4.0f;

	// Maximum history length that we need to store, in seconds
	static constexpr float MAX_HISTORY_SECONDS = 20.0f;

	// Maximum history length that we need to store, in samples
	static constexpr const size_t MAX_HISTORY_SAMPLES = size_t(MAX_HISTORY_SECONDS * FPS_UPDATES_PER_SECOND);

	void FPSDisplayWidget::create(GUIWindow* guiWindow)
	{
		Widget::create(guiWindow);

		m_fpsCache = 0.0f;
		m_fpsWaitFramesTilUpdate = 0;
		m_totalFpsSum = 0.0;
		m_totalFpsCount = 0ULL;
		m_totalFps = 0.0f;
		m_fps5s = 0.0f;
		m_fps10s = 0.0f;
		m_fps20s = 0.0f;
	}
	void FPSDisplayWidget::destroy()
	{
		m_fpsHistory.clear();

		Widget::destroy();
	}

	void FPSDisplayWidget::_render() const
	{
		PK_ASSERT_QUICK(isValid());

		// Update FPS if we have waited enough since the last FPS update
		m_fpsWaitFramesTilUpdate--;
		if (m_fpsWaitFramesTilUpdate < 1)
		{
			m_fpsCache = ImGui::GetIO().Framerate;
			m_fpsWaitFramesTilUpdate = int(m_fpsCache / FPS_UPDATES_PER_SECOND);

			// Store FPS in history
			m_fpsHistory.push_back(m_fpsCache);
			// Limit history size
			if (m_fpsHistory.size() > MAX_HISTORY_SAMPLES)
			{
				m_fpsHistory.pop_front();
			}

			// Update total stats
			m_totalFpsSum += m_fpsCache;
			m_totalFpsCount++;
			m_totalFps = float(m_totalFpsSum / double(m_totalFpsCount));
			// Update FPS for the last 5s, 10s and 20s
			m_fps5s = getAverageFpsOverSeconds(5.0f);
			m_fps10s = getAverageFpsOverSeconds(10.0f);
			m_fps20s = getAverageFpsOverSeconds(20.0f);
		}

		// Display current FPS
		ImGui::Text("%.3f ms/frame", 1000.0f / m_fpsCache);
		ImGui::Text("%.1f FPS", m_fpsCache);

		// Display averages
		ImGui::Separator();
		ImGui::Text("Avg FPS (5s):  %.1f", m_fps5s);
		ImGui::Text("Avg FPS (10s): %.1f", m_fps10s);
		ImGui::Text("Avg FPS (20s): %.1f", m_fps20s);
		ImGui::Text("Avg FPS (all): %.1f", m_totalFps);
	}

	float FPSDisplayWidget::getAverageFpsOverSeconds(const float seconds) const
	{
		const size_t samplesCount = static_cast<size_t>(seconds * FPS_UPDATES_PER_SECOND);
		PK_ASSERT_QUICK(samplesCount > 0);

		if (m_fpsHistory.size() < samplesCount)
		{
			return 0.0f;
		}

		double sum = 0.0;
		size_t count = 0ULL;
		const size_t startIndex = m_fpsHistory.size() - samplesCount;
		for (size_t i = startIndex; i < m_fpsHistory.size(); ++i)
		{
			sum += m_fpsHistory[i];
			count++;
		}
		if (count == 0)
		{
			PK_LOG_ERROR("Failed to calculate average FPS over " << seconds << " seconds in FPSDisplayWidget", "Pekan");
			return 0.0f;
		}

		const float avgFps = float(sum / double(count));
		return avgFps;
	}


} // namespace GUI
} // namespace Pekan

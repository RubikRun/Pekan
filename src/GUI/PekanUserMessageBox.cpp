#include "PekanUserMessageBox.h"

#include "PekanLogger.h"
#include "imgui.h"

#include <cstdio>
#include <mutex>
#include <queue>
#include <vector>
#include <cstdlib>
#include <cstring>

namespace Pekan
{
namespace GUI
{

#if PK_USER_MESSAGE_SUPPORT

	struct QueuedMessage
	{
		UserMessageType type;
		std::string message;
		unsigned id;            // Unique ID so ImGui can differentiate windows
	};

	// Protects s_messageQueue during multi-thread access.
	static std::mutex s_queueMutex;

	// A queue of messages to be displayed.
	static std::queue<QueuedMessage> s_messageQueue;

	// Messages currently being displayed as ImGui windows.
	static std::vector<QueuedMessage> s_messagesCurrentlyDisplayed;

	// Incremented for every queued message to ensure unique ImGui IDs.
	static unsigned s_nextMessageId = 0;

	// Minimum width of each message window (height auto-sizes to content).
	static constexpr float MODAL_MIN_WIDTH = 450.0f;

	// Offset applied to stacked windows so they don’t perfectly overlap.
	static constexpr float WINDOW_OFFSET = 40.0f;

	static bool getEnvVarBool(const char* varName, bool& exists)
	{
#ifdef _WIN32
		char* value = nullptr;
		size_t len = 0;
		if (_dupenv_s(&value, &len, varName) == 0 && value != nullptr)
		{
			exists = true;
			const bool result = (std::strcmp(value, "1") == 0);
			free(value);
			return result;
		}
		exists = false;
		return false;
#else
		const char* value = std::getenv(varName);
		if (value != nullptr)
		{
			exists = true;
			return (std::strcmp(value, "1") == 0);
		}
		exists = false;
		return false;
#endif
	}

#if PK_USER_MESSAGE_ERROR_SUPPORT
	static bool _isErrorEnabled()
	{
		bool exists = false;
		const bool envVal = getEnvVarBool("PEKAN_USER_MESSAGE_ERROR_ENABLED", exists);
		return exists ? envVal : static_cast<bool>(DEFAULT_PEKAN_USER_MESSAGE_ERROR_ENABLED);
	}
	static bool isErrorEnabled = _isErrorEnabled();
#endif

#if PK_USER_MESSAGE_WARNING_SUPPORT
	static bool _isWarningEnabled()
	{
		bool exists = false;
		const bool envVal = getEnvVarBool("PEKAN_USER_MESSAGE_WARNING_ENABLED", exists);
		return exists ? envVal : static_cast<bool>(DEFAULT_PEKAN_USER_MESSAGE_WARNING_ENABLED);
	}
	static bool isWarningEnabled = _isWarningEnabled();
#endif

#if PK_USER_MESSAGE_INFO_SUPPORT
	static bool _isInfoEnabled()
	{
		bool exists = false;
		const bool envVal = getEnvVarBool("PEKAN_USER_MESSAGE_INFO_ENABLED", exists);
		return exists ? envVal : static_cast<bool>(DEFAULT_PEKAN_USER_MESSAGE_INFO_ENABLED);
	}
	static bool isInfoEnabled = _isInfoEnabled();
#endif

	static const char* getTypeLabel(UserMessageType type)
	{
		switch (type)
		{
			case UserMessageType::Error:   return "Error";
			case UserMessageType::Warning: return "Warning";
			case UserMessageType::Info:    return "Info";
		}

		PK_LOG_ERROR("Unknown UserMessageType in getTypeLabel().", "Pekan");
		return "Message";
	}

	// Returns the color to be used for the window title bar for a given message type.
	static ImVec4 getColorForType(UserMessageType type)
	{
		switch (type)
		{
			case UserMessageType::Error:   return ImVec4(0.55f, 0.2f, 0.2f, 1.0f);
			case UserMessageType::Warning: return ImVec4(0.55f, 0.45f, 0.15f, 1.0f);
			case UserMessageType::Info:    return ImVec4(0.2f, 0.4f, 0.55f, 1.0f);
		}

		PK_LOG_ERROR("Unknown UserMessageType in getColorForType().", "Pekan");
		return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void queueUserMessage(UserMessageType type, const char* message)
	{
#if PK_USER_MESSAGE_ERROR_SUPPORT
		if (type == UserMessageType::Error && !isErrorEnabled)
		{
			return;
		}
#endif
#if PK_USER_MESSAGE_WARNING_SUPPORT
		if (type == UserMessageType::Warning && !isWarningEnabled)
		{
			return;
		}
#endif
#if PK_USER_MESSAGE_INFO_SUPPORT
		if (type == UserMessageType::Info && !isInfoEnabled)
		{
			return;
		}
#endif

		std::lock_guard<std::mutex> lock(s_queueMutex);

		QueuedMessage qm;
		qm.type = type;
		qm.message = (message != nullptr) ? message : "";
		qm.id = s_nextMessageId++;

		s_messageQueue.push(std::move(qm));
	}

	// Renders all pending user message windows.
	void renderPendingUserMessages()
	{
		// STEP 1: Transfer queued messages to GUI-owned storage.
		//
		// We briefly lock the mutex to safely move messages into s_messagesCurrentlyDisplayed.
		//
		// After this block:
		//  - The queue is empty.
		//  - Only this function touches s_messagesCurrentlyDisplayed.
		//  - No further locking is required during rendering.
		{
			std::lock_guard<std::mutex> lock(s_queueMutex);

			while (!s_messageQueue.empty())
			{
				s_messagesCurrentlyDisplayed.push_back(std::move(s_messageQueue.front()));
				s_messageQueue.pop();
			}
		}

		if (s_messagesCurrentlyDisplayed.empty())
			return;

		// In this vector we will store the indices of the windows that need to be removed this frame.
		std::vector<size_t> toRemove;

		// STEP 2: Render one window per message in s_messagesCurrentlyDisplayed
		//
		// These are regular ImGui windows (not modal popups).
		// That means:
		//  - The rest of the UI remains interactive.
		//  - Each message behaves like a floating notification window.
		//
		// Windows are slightly offset so multiple messages don’t completely overlap visually.
		for (size_t i = 0; i < s_messagesCurrentlyDisplayed.size(); ++i)
		{
			const QueuedMessage& msg = s_messagesCurrentlyDisplayed[i];
			const char* typeLabel = getTypeLabel(msg.type);

			char idBuf[64];
			std::snprintf(idBuf, sizeof(idBuf), "%s##UserMessage_%u", typeLabel, msg.id);

			// Position window near screen center with stacking offset
			const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			const ImVec2 pos(center.x + float(i) * WINDOW_OFFSET,
			           center.y + float(i) * WINDOW_OFFSET);
			ImGui::SetNextWindowSize(ImVec2(MODAL_MIN_WIDTH, 0.0f));
			ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			const ImVec4 titleBarColor = getColorForType(msg.type);
			// Color all title bar states for consistent appearance.
			ImGui::PushStyleColor(ImGuiCol_TitleBg, titleBarColor);
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, titleBarColor);
			ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, titleBarColor);

			// Begin the window.
			// If the window is opened, render the message and an OK button.
			if (ImGui::Begin(idBuf, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
			{
				// Render the message
				ImGui::TextWrapped("%s", msg.message.c_str());

				// Add some spacing between the message and the button
				ImGui::Spacing();

				// Render the OK button.
				// If the button is clicked, add the index of the message to the vector of indices to remove.
				if (ImGui::Button("OK", ImVec2(120, 0)))
				{
					toRemove.push_back(i);
				}
			}
			// End the window
			ImGui::End();

			// Pop the style colors for the title bar
			ImGui::PopStyleColor(3);
		}

		// STEP 3: Remove dismissed messages.
		//
		// We erase from highest index to lowest. This prevents index invalidation during vector erase().
		for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it)
		{
			s_messagesCurrentlyDisplayed.erase(s_messagesCurrentlyDisplayed.begin() + static_cast<ptrdiff_t>(*it));
		}
	}

#endif // PK_USER_MESSAGE_SUPPORT

} // namespace GUI
} // namespace Pekan
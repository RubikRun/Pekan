#pragma once

#include <sstream>

// Toggle these macros on/off to enable/disable support for different types of user messages.
//
// NOTE: These macros give compile-time control over the user message display.
//       Setting PK_USER_MESSAGE_ERROR_SUPPORT 0 removes all PK_SHOW_ERROR code from the codebase.
//
// NOTE: For user-level control we support corresponding environment variables
//           PEKAN_USER_MESSAGE_ERROR_ENABLED
//           PEKAN_USER_MESSAGE_WARNING_ENABLED
//           PEKAN_USER_MESSAGE_INFO_ENABLED
//       so if built with PK_USER_MESSAGE_ERROR_SUPPORT 1, the user can disable at runtime with
//           set PEKAN_USER_MESSAGE_ERROR_ENABLED=0
//
#define PK_USER_MESSAGE_ERROR_SUPPORT 1
#define PK_USER_MESSAGE_WARNING_SUPPORT 1
#define PK_USER_MESSAGE_INFO_SUPPORT 1

// Default values for the environment variables
#define DEFAULT_PEKAN_USER_MESSAGE_ERROR_ENABLED 1
#define DEFAULT_PEKAN_USER_MESSAGE_WARNING_ENABLED 1
#define DEFAULT_PEKAN_USER_MESSAGE_INFO_ENABLED 1

#define PK_USER_MESSAGE_SUPPORT (PK_USER_MESSAGE_ERROR_SUPPORT || PK_USER_MESSAGE_WARNING_SUPPORT || PK_USER_MESSAGE_INFO_SUPPORT)

namespace Pekan
{
namespace GUI
{

#if PK_USER_MESSAGE_SUPPORT

	enum class UserMessageType
	{
		Error,
		Warning,
		Info
	};

	// Queues a message to be displayed in an ImGui window.
	// Thread-safe. Call from anywhere. Messages are shown when renderPendingUserMessages() runs.
	void queueUserMessage(UserMessageType type, const char* message);

	// Renders all pending user message windows.
	// Must be called from the GUI thread, between ImGui::NewFrame() and ImGui::Render().
	// Typically called from GUISubsystem::endFrame().
	void renderPendingUserMessages();

#endif // PK_USER_MESSAGE_SUPPORT

} // namespace GUI
} // namespace Pekan

#define PK_STR_USER_MSG(MSG) std::ostringstream _pk_um_oss; _pk_um_oss << MSG; const std::string _pk_um_m = _pk_um_oss.str();

#if PK_USER_MESSAGE_ERROR_SUPPORT
	#define PK_SHOW_ERROR(MSG) \
		do { PK_STR_USER_MSG(MSG); Pekan::GUI::queueUserMessage(Pekan::GUI::UserMessageType::Error, _pk_um_m.c_str()); } while (false)
#else
	#define PK_SHOW_ERROR(MSG)
#endif

#if PK_USER_MESSAGE_WARNING_SUPPORT
	#define PK_SHOW_WARNING(MSG) \
		do { PK_STR_USER_MSG(MSG); Pekan::GUI::queueUserMessage(Pekan::GUI::UserMessageType::Warning, _pk_um_m.c_str()); } while (false)
#else
	#define PK_SHOW_WARNING(MSG)
#endif

#if PK_USER_MESSAGE_INFO_SUPPORT
	#define PK_SHOW_INFO(MSG) \
		do { PK_STR_USER_MSG(MSG); Pekan::GUI::queueUserMessage(Pekan::GUI::UserMessageType::Info, _pk_um_m.c_str()); } while (false)
#else
	#define PK_SHOW_INFO(MSG)
#endif

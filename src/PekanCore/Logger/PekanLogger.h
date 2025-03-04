#pragma once

#include <sstream>
#include <string_view>

// Toggle this macro on/off to log filepaths/filenames of source files.
// e.g. when on:
//     [ERROR in C:/dev/Pekan/src/PekanRenderer/Shader.cpp]: sth
// when off:
//     [ERROR in Shader.cpp]: sth
#define PK_LOGGER_USE_FILEPATH_FOR_SOURCE_FILE 0

// Toggle these macros on/off to enable/disable support of different types of log messages
// or enable/disable support for logging to the console and logging to a file.
// 
// NOTE: These macros give compile-time control over the logging.
//       So for example setting
//           PK_LOGGER_ERROR_SUPPORT 0
//       removes all error logging code from the whole code base,
//       which is perfect for performance.
//       However, after building Pekan Engine or a Pekan Application with
//           PK_LOGGER_ERROR_SUPPORT 0
//       there is no way for user to enable error logging.
//       In practice, we would almost always want to have these macros enabled,
//       but still there is an option to disable them
//       just to remove logging code entirely, for optimization purposes.
// 
// NOTE: For user-level control we support corresponding environment variables
//           PEKAN_LOGGER_ERROR_ENABLED
//           PEKAN_LOGGER_WARNING_ENABLED
//           PEKAN_LOGGER_INFO_ENABLED
//           PEKAN_LOGGER_DEBUG_ENABLED
//           PEKAN_LOGGER_CONSOLE_ENABLED
//           PEKAN_LOGGER_FILE_ENABLED
//       so if Pekan Engine or a Pekan Application is built with
//           PK_LOGGER_ERROR_SUPPORT 1
//       then user can disable error logging by setting this environment variable to 0
//           set PEKAN_LOGGER_ERROR_ENABLED=0
//       However, if a build is done with
//           PK_LOGGER_ERROR_SUPPORT 0
//       then there is no way for user to enable error logging.
//
#define PK_LOGGER_ERROR_SUPPORT 1
#define PK_LOGGER_WARNING_SUPPORT 1
#define PK_LOGGER_INFO_SUPPORT 1
#define PK_LOGGER_DEBUG_SUPPORT 1
#define PK_LOGGER_CONSOLE_SUPPORT 1
#define PK_LOGGER_FILE_SUPPORT 1

// Default values for the environment variables
//     PEKAN_LOGGER_ERROR_ENABLED
//     PEKAN_LOGGER_WARNING_ENABLED
//     PEKAN_LOGGER_INFO_ENABLED
//     PEKAN_LOGGER_DEBUG_ENABLED
//     PEKAN_LOGGER_CONSOLE_ENABLED
//     PEKAN_LOGGER_FILE_ENABLED
// These values determine the default enabled/disabled state of different types of log messages
// and the default enabled/disabled state of logging to the console and logging to a file.
// 
// So if Pekan Engine or a Pekan Application is built with
//     PK_LOGGER_ERROR_SUPPORT 1
// and we have error logging disabled by default
//     DEFAULT_PEKAN_LOGGER_ERROR_ENABLED 0
// this means that error logging will be disabled
// unless user enables it by setting this environment variable to 1
//     set PEKAN_LOGGER_ERROR_ENABLED=1
//
#define DEFAULT_PEKAN_LOGGER_ERROR_ENABLED 1
#define DEFAULT_PEKAN_LOGGER_WARNING_ENABLED 1
#define DEFAULT_PEKAN_LOGGER_INFO_ENABLED 1
#define DEFAULT_PEKAN_LOGGER_DEBUG_ENABLED 0
#define DEFAULT_PEKAN_LOGGER_CONSOLE_ENABLED 1
#define DEFAULT_PEKAN_LOGGER_FILE_ENABLED 0

// Toggle these macros on/off to include/exclude source file's name from different types of log messages.
#define PK_LOGGER_ERRORS_INCLUDE_SOURCE_FILE 1
#define PK_LOGGER_WARNINGS_INCLUDE_SOURCE_FILE 1
#define PK_LOGGER_INFOS_INCLUDE_SOURCE_FILE 0
#define PK_LOGGER_DEBUGS_INCLUDE_SOURCE_FILE 1

#define PK_LOGGER_SUPPORT ((PK_LOGGER_ERROR_SUPPORT || PK_LOGGER_WARNING_SUPPORT || PK_LOGGER_INFO_SUPPORT || PK_LOGGER_DEBUG_SUPPORT) && (PK_LOGGER_CONSOLE_SUPPORT || PK_LOGGER_FILE_SUPPORT))

namespace Pekan
{
namespace Logger
{

#if PK_LOGGER_CONSOLE_SUPPORT
	#if PK_LOGGER_ERROR_SUPPORT
		#if PK_LOGGER_ERRORS_INCLUDE_SOURCE_FILE
			void _logErrorToConsole(const char* msg, std::string_view sourceFileName);
		#else
			void _logErrorToConsole(const char* msg);
		#endif
	#endif
	#if PK_LOGGER_WARNING_SUPPORT
		#if PK_LOGGER_WARNINGS_INCLUDE_SOURCE_FILE
			void _logWarningToConsole(const char* msg, std::string_view sourceFileName);
		#else
			void _logWarningToConsole(const char* msg);
		#endif
	#endif
	#if PK_LOGGER_INFO_SUPPORT
		#if PK_LOGGER_INFOS_INCLUDE_SOURCE_FILE
			void _logInfoToConsole(const char* msg, std::string_view sourceFileName);
		#else
			void _logInfoToConsole(const char* msg);
		#endif
	#endif
	#if PK_LOGGER_DEBUG_SUPPORT
		#if PK_LOGGER_DEBUGS_INCLUDE_SOURCE_FILE
			void _logDebugToConsole(const char* msg, std::string_view sourceFileName);
		#else
			void _logDebugToConsole(const char* msg);
		#endif
	#endif
#endif

#if PK_LOGGER_FILE_SUPPORT
	#if PK_LOGGER_ERROR_SUPPORT
		#if PK_LOGGER_ERRORS_INCLUDE_SOURCE_FILE
			void _logErrorToFile(const char* msg, std::string_view sourceFileName);
		#else
			void _logErrorToFile(const char* msg);
		#endif
	#endif
	#if PK_LOGGER_WARNING_SUPPORT
		#if PK_LOGGER_WARNINGS_INCLUDE_SOURCE_FILE
			void _logWarningToFile(const char* msg, std::string_view sourceFileName);
		#else
			void _logWarningToFile(const char* msg);
		#endif
	#endif
	#if PK_LOGGER_INFO_SUPPORT
		#if PK_LOGGER_INFOS_INCLUDE_SOURCE_FILE
			void _logInfoToFile(const char* msg, std::string_view sourceFileName);
		#else
			void _logInfoToFile(const char* msg);
		#endif
	#endif
	#if PK_LOGGER_DEBUG_SUPPORT
		#if PK_LOGGER_DEBUGS_INCLUDE_SOURCE_FILE
			void _logDebugToFile(const char* msg, std::string_view sourceFileName);
		#else
			void _logDebugToFile(const char* msg);
		#endif
	#endif
#endif

#if PK_LOGGER_SUPPORT
	#if PK_LOGGER_USE_FILEPATH_FOR_SOURCE_FILE
		// Filepath of current source file where logger is used
		#define PK_SOURCE_FILE __FILE__
	#else
		// Extracts a filename from given filepath,
		// e.g. "C:/dev/Pekan/src/PekanRenderer/Shader.cpp" -> "Shader.cpp".
		constexpr std::string_view _getFileNameFromFilePath(std::string_view filePath)
		{
			const size_t pos = filePath.find_last_of("/\\");
			return (pos == std::string_view::npos) ? filePath : filePath.substr(pos + 1);
		}
		// Filename of current source file where logger is used
		#define PK_SOURCE_FILE Pekan::Logger::_getFileNameFromFilePath(__FILE__)
	#endif
#endif

} // namespace Logger
} // namespace Pekan

#define PK_STR(MSG) std::ostringstream oss; oss << MSG; const std::string s = oss.str();

// PK_LOG_ERROR logs an error message to the console and/or syslog file
#if PK_LOGGER_ERROR_SUPPORT
	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_ERRORS_INCLUDE_SOURCE_FILE
			#define PK_LOG_ERROR(MSG) { PK_STR(MSG); Pekan::Logger::_logErrorToConsole(s.c_str(), PK_SOURCE_FILE); Pekan::Logger::_logErrorToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_ERROR(MSG) { PK_STR(MSG); Pekan::Logger::_logErrorToConsole(s.c_str()); Pekan::Logger::_logErrorToFile(s.c_str()); }
		#endif
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#if PK_LOGGER_ERRORS_INCLUDE_SOURCE_FILE
			#define PK_LOG_ERROR(MSG) { PK_STR(MSG); Pekan::Logger::_logErrorToConsole(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_ERROR(MSG) { PK_STR(MSG); Pekan::Logger::_logErrorToConsole(s.c_str()); }
		#endif
	#elif PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_ERRORS_INCLUDE_SOURCE_FILE
			#define PK_LOG_ERROR(MSG) { PK_STR(MSG); Pekan::Logger::_logErrorToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_ERROR(MSG) { PK_STR(MSG); Pekan::Logger::_logErrorToFile(s.c_str()); }
		#endif
	#else
		#define PK_LOG_ERROR(MSG)
	#endif
#else
	#define PK_LOG_ERROR(MSG)
#endif

// PK_LOG_WARNING logs a warning message to the console and/or syslog file
#if PK_LOGGER_WARNING_SUPPORT
	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_WARNINGS_INCLUDE_SOURCE_FILE
			#define PK_LOG_WARNING(MSG) { PK_STR(MSG); Pekan::Logger::_logWarningToConsole(s.c_str(), PK_SOURCE_FILE); Pekan::Logger::_logWarningToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_WARNING(MSG) { PK_STR(MSG); Pekan::Logger::_logWarningToConsole(s.c_str()); Pekan::Logger::_logWarningToFile(s.c_str()); }
		#endif
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#if PK_LOGGER_WARNINGS_INCLUDE_SOURCE_FILE
			#define PK_LOG_WARNING(MSG) { PK_STR(MSG); Pekan::Logger::_logWarningToConsole(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_WARNING(MSG) { PK_STR(MSG); Pekan::Logger::_logWarningToConsole(s.c_str()); }
		#endif
	#elif PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_WARNINGS_INCLUDE_SOURCE_FILE
			#define PK_LOG_WARNING(MSG) { PK_STR(MSG); Pekan::Logger::_logWarningToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_WARNING(MSG) { PK_STR(MSG); Pekan::Logger::_logWarningToFile(s.c_str()); }
		#endif
	#else
		#define PK_LOG_WARNING(MSG)
	#endif
#else
	#define PK_LOG_WARNING(MSG)
#endif

// PK_LOG_INFO logs an info message to the console and/or syslog file
#if PK_LOGGER_INFO_SUPPORT
	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_INFOS_INCLUDE_SOURCE_FILE
			#define PK_LOG_INFO(MSG) { PK_STR(MSG); Pekan::Logger::_logInfoToConsole(s.c_str(), PK_SOURCE_FILE); Pekan::Logger::_logInfoToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_INFO(MSG) { PK_STR(MSG); Pekan::Logger::_logInfoToConsole(s.c_str()); Pekan::Logger::_logInfoToFile(s.c_str()); }
		#endif
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#if PK_LOGGER_INFOS_INCLUDE_SOURCE_FILE
			#define PK_LOG_INFO(MSG) { PK_STR(MSG); Pekan::Logger::_logInfoToConsole(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_INFO(MSG) { PK_STR(MSG); Pekan::Logger::_logInfoToConsole(s.c_str()); }
		#endif
	#elif PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_INFOS_INCLUDE_SOURCE_FILE
			#define PK_LOG_INFO(MSG) { PK_STR(MSG); Pekan::Logger::_logInfoToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_INFO(MSG) { PK_STR(MSG); Pekan::Logger::_logInfoToFile(s.c_str()); }
		#endif
	#else
		#define PK_LOG_INFO(MSG)
	#endif
#else
	#define PK_LOG_INFO(MSG)
#endif

// PK_LOG_DEBUG logs a debug message to the console and/or syslog file
#if PK_LOGGER_DEBUG_SUPPORT
	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_DEBUGS_INCLUDE_SOURCE_FILE
			#define PK_LOG_DEBUG(MSG) { PK_STR(MSG); Pekan::Logger::_logDebugToConsole(s.c_str(), PK_SOURCE_FILE); Pekan::Logger::_logDebugToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_DEBUG(MSG) { PK_STR(MSG); Pekan::Logger::_logDebugToConsole(s.c_str()); Pekan::Logger::_logDebugToFile(s.c_str()); }
		#endif
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#if PK_LOGGER_DEBUGS_INCLUDE_SOURCE_FILE
			#define PK_LOG_DEBUG(MSG) { PK_STR(MSG); Pekan::Logger::_logDebugToConsole(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_DEBUG(MSG) { PK_STR(MSG); Pekan::Logger::_logDebugToConsole(s.c_str()); }
		#endif
	#elif PK_LOGGER_FILE_SUPPORT
		#if PK_LOGGER_DEBUGS_INCLUDE_SOURCE_FILE
			#define PK_LOG_DEBUG(MSG) { PK_STR(MSG); Pekan::Logger::_logDebugToFile(s.c_str(), PK_SOURCE_FILE); }
		#else
			#define PK_LOG_DEBUG(MSG) { PK_STR(MSG); Pekan::Logger::_logDebugToFile(s.c_str()); }
		#endif
	#else
		#define PK_LOG_DEBUG(MSG)
	#endif
#else
	#define PK_LOG_DEBUG(MSG)
#endif

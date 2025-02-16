#pragma once

/*
Before including this file you must define PK_FILENAME to be the name of your file (e.g. #define PK_FILENAME "main.cpp")
if you are planning to use PK_LOG_ERRORF, PK_LOG_WARNINGF, PK_LOG_DEBUGF, PK_LOG_INFOF.
*/

#include <sstream>

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

namespace Pekan
{
	namespace Logger
	{

#if PK_LOGGER_CONSOLE_SUPPORT
	#if PK_LOGGER_ERROR_SUPPORT
		void _logErrorToConsole(const char* msg);
		void _logErrorfToConsole(const char* msg, const char* filename);
	#endif
	#if PK_LOGGER_WARNING_SUPPORT
		void _logWarningToConsole(const char* msg);
		void _logWarningfToConsole(const char* msg, const char* filename);
	#endif
	#if PK_LOGGER_INFO_SUPPORT
		void _logInfoToConsole(const char* msg);
		void _logInfofToConsole(const char* msg, const char* filename);
	#endif
	#if PK_LOGGER_DEBUG_SUPPORT
		void _logDebugToConsole(const char* msg);
		void _logDebugfToConsole(const char* msg, const char* filename);
	#endif
#endif

#if PK_LOGGER_FILE_SUPPORT
	#if PK_LOGGER_ERROR_SUPPORT
		void _logErrorToFile(const char* msg);
		void _logErrorfToFile(const char* msg, const char* filename);
	#endif
	#if PK_LOGGER_WARNING_SUPPORT
		void _logWarningToFile(const char* msg);
		void _logWarningfToFile(const char* msg, const char* filename);
	#endif
	#if PK_LOGGER_INFO_SUPPORT
		void _logInfoToFile(const char* msg);
		void _logInfofToFile(const char* msg, const char* filename);
	#endif
	#if PK_LOGGER_DEBUG_SUPPORT
		void _logDebugToFile(const char* msg);
		void _logDebugfToFile(const char* msg, const char* filename);
	#endif
#endif

	} // namespace Logger
} // namespace Pekan

// PK_LOG_ERROR logs an error message to the console and/or syslog file
// PK_LOG_ERRORF logs an error message to the console and/or syslog file together with the filename of the source file where it's called from
#if PK_LOGGER_ERROR_SUPPORT

	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_ERROR(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logErrorToConsole(s.c_str()); Pekan::Logger::_logErrorToFile(s.c_str()); }
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#define PK_LOG_ERROR(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logErrorToConsole(s.c_str()); }
	#elif PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_ERROR(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logErrorToFile(s.c_str()); }
	#else
		#define PK_LOG_ERROR(MSG)
	#endif

	#ifdef PK_FILENAME
		#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_ERRORF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logErrorfToConsole(s.c_str(), PK_FILENAME); Pekan::Logger::_logErrorfToFile(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_CONSOLE_SUPPORT
			#define PK_LOG_ERRORF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logErrorfToConsole(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_ERRORF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logErrorfToFile(s.c_str(), PK_FILENAME); }
		#else
			#define PK_LOG_ERRORF(MSG)
		#endif
	#else
		#define PK_LOG_ERRORF(MSG) ERROR You must define PK_FILENAME to be the name of your file (e.g. {hashtag}define PK_FILENAME "main.cpp")
	#endif

#else
	#define PK_LOG_ERROR(MSG)
	#define PK_LOG_ERRORF(MSG)
#endif

// PK_LOG_WARNING logs a warning message to the console and/or syslog file
// PK_LOG_WARNINGF logs a warning message to the console and/or syslog file together with the filename of the source file where it's called from
#if PK_LOGGER_WARNING_SUPPORT

	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_WARNING(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logWarningToConsole(s.c_str()); Pekan::Logger::_logWarningToFile(s.c_str()); }
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#define PK_LOG_WARNING(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logWarningToConsole(s.c_str()); }
	#elif PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_WARNING(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logWarningToFile(s.c_str()); }
	#else
		#define PK_LOG_WARNING(MSG)
	#endif

	#ifdef PK_FILENAME
		#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_WARNINGF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logWarningfToConsole(s.c_str(), PK_FILENAME); Pekan::Logger::_logWarningfToFile(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_CONSOLE_SUPPORT
			#define PK_LOG_WARNINGF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logWarningfToConsole(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_WARNINGF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logWarningfToFile(s.c_str(), PK_FILENAME); }
		#else
			#define PK_LOG_WARNINGF(MSG)
		#endif
	#else
		#define PK_LOG_WARNINGF(MSG) ERROR You must define PK_FILENAME to be the name of your file (e.g. {hashtag}define PK_FILENAME "main.cpp")
	#endif

#else
	#define PK_LOG_WARNING(MSG)
	#define PK_LOG_WARNINGF(MSG)
#endif

// PK_LOG_INFO logs an info message to the console and/or syslog file
// PK_LOG_INFOF logs an info message to the console and/or syslog file together with the filename of the source file where it's called from
#if PK_LOGGER_INFO_SUPPORT

	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_INFO(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logInfoToConsole(s.c_str()); Pekan::Logger::_logInfoToFile(s.c_str()); }
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#define PK_LOG_INFO(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logInfoToConsole(s.c_str()); }
	#elif PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_INFO(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logInfoToFile(s.c_str()); }
	#else
		#define PK_LOG_INFO(MSG)
	#endif

	#ifdef PK_FILENAME
		#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_INFOF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logInfofToConsole(s.c_str(), PK_FILENAME); Pekan::Logger::_logInfofToFile(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_CONSOLE_SUPPORT
			#define PK_LOG_INFOF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logInfofToConsole(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_INFOF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logInfofToFile(s.c_str(), PK_FILENAME); }
		#else
			#define PK_LOG_INFOF(MSG)
		#endif
	#else
		#define PK_LOG_INFOF(MSG) ERROR You must define PK_FILENAME to be the name of your file (e.g. {hashtag}define PK_FILENAME "main.cpp")
	#endif

#else
	#define PK_LOG_INFO(MSG)
	#define PK_LOG_INFOF(MSG)
#endif

// PK_LOG_DEBUG logs a debug message to the console and/or syslog file
// PK_LOG_DEBUGF logs a debug message to the console and/or syslog file together with the filename of the source file where it's called from
#if PK_LOGGER_DEBUG_SUPPORT

	#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_DEBUG(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logDebugToConsole(s.c_str()); Pekan::Logger::_logDebugToFile(s.c_str()); }
	#elif PK_LOGGER_CONSOLE_SUPPORT
		#define PK_LOG_DEBUG(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logDebugToConsole(s.c_str()); }
	#elif PK_LOGGER_FILE_SUPPORT
		#define PK_LOG_DEBUG(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logDebugToFile(s.c_str()); }
	#else
		#define PK_LOG_DEBUG(MSG)
	#endif

	#ifdef PK_FILENAME
		#if PK_LOGGER_CONSOLE_SUPPORT && PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_DEBUGF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logDebugfToConsole(s.c_str(), PK_FILENAME); Pekan::Logger::_logDebugfToFile(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_CONSOLE_SUPPORT
			#define PK_LOG_DEBUGF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logDebugfToConsole(s.c_str(), PK_FILENAME); }
		#elif PK_LOGGER_FILE_SUPPORT
			#define PK_LOG_DEBUGF(MSG) { std::ostringstream oss; oss << MSG; const std::string s = oss.str(); Pekan::Logger::_logDebugfToFile(s.c_str(), PK_FILENAME); }
		#else
			#define PK_LOG_DEBUGF(MSG)
		#endif
	#else
		#define PK_LOG_DEBUGF(MSG) ERROR You must define PK_FILENAME to be the name of your file (e.g. {hashtag}define PK_FILENAME "main.cpp")
	#endif

#else
	#define PK_LOG_DEBUG(MSG)
	#define PK_LOG_DEBUGF(MSG)
#endif

#include "PekanLogger.h"

#include <iostream>
#include <filesystem>
#include <fstream>

#define SPACES_STR(N) std::string(N, ' ')

namespace Pekan
{
namespace Logger
{

#if PK_LOGGER_SUPPORT

    // Returns the value of an environment variable,
    // or an empty string if given environment variable doesn't exist.
    static std::string getEnvVar(const std::string& varName)
    {
        const char* value = std::getenv(varName.c_str());
        return (value == nullptr) ? "" : std::string(value);
    }

    // Returns a bool value from the value of a an environment variable.
    // If environment variable exists and has a value "1" then true will be returned, and "exists" flag will be set to true.
    // If environment variable exists and has a value "0" then false will be returned, and "exists" flag will be set to true.
    // If environment variable doesn't exist or has any other value, then "exists" flag will be set to false.
    static bool getEnvVarBool(const std::string& varName, bool &exists)
    {
        const std::string s = getEnvVar(varName);
        if (s == "1")
        {
            exists = true;
            return true;
        }
        if (s == "0")
        {
            exists = true;
            return false;
        }
        exists = false;
        return false;
    }

#if PK_LOGGER_FILE_SUPPORT
    // Checks if a given directory path ends in a slash. If not, adds a slash to the end.
    // It adds the preferred slash type (/ or \) depending on OS.
    static std::string ensureTrailingSlash(const std::string& path)
    {
        if (path.empty())
        {
            return path;
        }

        const char lastChar = path.back();
        // If there is already a trailing slash, just return path
        if (lastChar == '/' || lastChar == '\\')
        {
            return path;
        }
        // Otherwise append the preferred slash type to the end
        return path + char(std::filesystem::path::preferred_separator);
    }

    // Returns path to log directory.
    // Log directory can be specified with this environment variable
    //     PEKAN_LOG_DIR
    // If environment variable is not set, then log directory will be OS's temporary directory.
    // Returned directory path will have a trailing slash.
    static std::string getLogDir()
    {
        std::string logDir = getEnvVar("PEKAN_LOG_DIR");
        if (logDir.empty())
        {
            logDir = std::filesystem::temp_directory_path().string();
        }
        if (logDir.empty())
        {
            return std::string();
        }
        logDir = ensureTrailingSlash(logDir);
        return logDir;
    }

    // Returns current date-time as a string formatted like "YYYYmmddHHMMSS", for example "20250207194915"
    static std::string getCurrentDateTimeStringId()
    {
        // Get current time
        const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        const std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        // Convert to local time
        std::tm localTime;
#ifdef _WIN32
        localtime_s(&localTime, &now_c);
#else
        localtime_r(&now_c, &localTime);
#endif

        // Format into a string
        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y%m%d%H%M%S");

        return oss.str();
    }

    // Returns filepath to log file.
    // Log file will be in PEKAN_LOG_DIR or in OS's temporary directory
    // and will be named "pekanYYYYmmddHHMMSS.log" where YYYYmmddHHMMSS is the current date-time.
    // WARNING: Do not call directly. Instead use static variable
    //              logFilePath
    //          This function is to be used only once for the value of that variable.
    static std::string _getLogFilePath()
    {
        const std::string logDir = getLogDir();
        const std::string dateTimeId = getCurrentDateTimeStringId();
        return logDir + "pekan" + dateTimeId + ".log";
    }
    static const std::string logFilePath = _getLogFilePath();

#endif // PK_LOGGER_FILE_SUPPORT

#if PK_LOGGER_ERROR_SUPPORT
    // Checks if error logging is currently enabled,
    // either by an environment variable or by its default state.
    static bool _isErrorEnabled()
    {
        bool envVarExists = false;
        const bool envVarValue = getEnvVarBool("PEKAN_LOGGER_ERROR_ENABLED", envVarExists);
        if (envVarExists)
        {
            return envVarValue;
        }
        return DEFAULT_PEKAN_LOGGER_ERROR_ENABLED;
    }
    static bool isErrorEnabled = _isErrorEnabled();
#endif // PK_LOGGER_ERROR_SUPPORT
#if PK_LOGGER_WARNING_SUPPORT
    // Checks if warning logging is currently enabled,
    // either by an environment variable or by its default state.
    static bool _isWarningEnabled()
    {
        bool envVarExists = false;
        const bool envVarValue = getEnvVarBool("PEKAN_LOGGER_WARNING_ENABLED", envVarExists);
        if (envVarExists)
        {
            return envVarValue;
        }
        return DEFAULT_PEKAN_LOGGER_WARNING_ENABLED;
    }
    static bool isWarningEnabled = _isWarningEnabled();
#endif // PK_LOGGER_WARNING_SUPPORT
#if PK_LOGGER_INFO_SUPPORT
    // Checks if info logging is currently enabled,
    // either by an environment variable or by its default state.
    static bool _isInfoEnabled()
    {
        bool envVarExists = false;
        const bool envVarValue = getEnvVarBool("PEKAN_LOGGER_INFO_ENABLED", envVarExists);
        if (envVarExists)
        {
            return envVarValue;
        }
        return DEFAULT_PEKAN_LOGGER_INFO_ENABLED;
    }
    static bool isInfoEnabled = _isInfoEnabled();
#endif // PK_LOGGER_INFO_SUPPORT
#if PK_LOGGER_DEBUG_SUPPORT
    // Checks if debug logging is currently enabled,
    // either by an environment variable or by its default state.
    static bool _isDebugEnabled()
    {
        bool envVarExists = false;
        const bool envVarValue = getEnvVarBool("PEKAN_LOGGER_DEBUG_ENABLED", envVarExists);
        if (envVarExists)
        {
            return envVarValue;
        }
        return DEFAULT_PEKAN_LOGGER_DEBUG_ENABLED;
    }
    static bool isDebugEnabled = _isDebugEnabled();
#endif // PK_LOGGER_DEBUG_SUPPORT
#if PK_LOGGER_CONSOLE_SUPPORT
    // Checks if console logging is currently enabled,
    // either by an environment variable or by its default state.
    static bool _isConsoleEnabled()
    {
        bool envVarExists = false;
        const bool envVarValue = getEnvVarBool("PEKAN_LOGGER_CONSOLE_ENABLED", envVarExists);
        if (envVarExists)
        {
            return envVarValue;
        }
        return DEFAULT_PEKAN_LOGGER_CONSOLE_ENABLED;
    }
    static bool isConsoleEnabled = _isConsoleEnabled();
#endif // PK_LOGGER_CONSOLE_SUPPORT
#if PK_LOGGER_FILE_SUPPORT
    // Checks if file logging is currently enabled,
    // either by an environment variable or by its default state.
    static bool _isFileEnabled()
    {
        bool envVarExists = false;
        const bool envVarValue = getEnvVarBool("PEKAN_LOGGER_FILE_ENABLED", envVarExists);
        if (envVarExists)
        {
            return envVarValue;
        }
        return DEFAULT_PEKAN_LOGGER_FILE_ENABLED;
    }
    static bool isFileEnabled = _isFileEnabled();
#endif // PK_LOGGER_FILE_SUPPORT

#if PK_LOGGER_CONSOLE_SUPPORT

#if PK_LOGGER_ERROR_SUPPORT
    void _logErrorToConsole(const char* msg, const char* sender)
    {
        if (isConsoleEnabled && isErrorEnabled)
        {
            std::cout << "[ERROR](" << sender << "): " << msg << std::endl;
        }
    }

    void _logErrorToConsole(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isConsoleEnabled && isErrorEnabled)
        {
            std::cout << "[ERROR in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;
        }
    }
#endif // PK_LOGGER_ERROR_SUPPORT
#if PK_LOGGER_WARNING_SUPPORT
    void _logWarningToConsole(const char* msg, const char* sender)
    {
        if (isConsoleEnabled && isWarningEnabled)
        {
            std::cout << "[WARNING](" << sender << "): " << msg << std::endl;
        }
    }

    void _logWarningToConsole(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isConsoleEnabled && isWarningEnabled)
        {
            std::cout << "[WARNING in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;
        }
    }
#endif // PK_LOGGER_WARNING_SUPPORT
#if PK_LOGGER_INFO_SUPPORT
    void _logInfoToConsole(const char* msg, const char* sender)
    {
        if (isConsoleEnabled && isInfoEnabled)
        {
            std::cout << "[INFO](" << sender << "): " << msg << std::endl;
        }
    }

    void _logInfoToConsole(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isConsoleEnabled && isInfoEnabled)
        {
            std::cout << "[INFO in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;
        }
    }
#endif // PK_LOGGER_INFO_SUPPORT
#if PK_LOGGER_DEBUG_SUPPORT
    void _logDebugToConsole(const char* msg, const char* sender)
    {
        if (isConsoleEnabled && isDebugEnabled)
        {
            std::cout << "[DEBUG](" << sender << "): " << msg << std::endl;
        }
    }

    void _logDebugToConsole(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isConsoleEnabled && isDebugEnabled)
        {
            std::cout << "[DEBUG in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;
        }
    }
#endif // PK_LOGGER_DEBUG_SUPPORT

#endif // PK_LOGGER_CONSOLE_SUPPORT

#if PK_LOGGER_FILE_SUPPORT

#if PK_LOGGER_ERROR_SUPPORT
    void _logErrorToFile(const char* msg, const char* sender)
    {
        if (isFileEnabled && isErrorEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[ERROR][Failed to write this message to log file](" << sender << "): " << msg << std::endl;
                return;
            }

            logFile << "[ERROR](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[ERROR][Failed to close log file after writing message](" << sender << "): " << msg << std::endl;
            }
        }
    }

    void _logErrorToFile(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isFileEnabled && isErrorEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[ERROR in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to write this message to log file]: " << msg << std::endl;
                return;
            }

            logFile << "[ERROR in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[ERROR in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to close log file after writing message]: " << msg << std::endl;
            }
        }
    }
#endif // PK_LOGGER_ERROR_SUPPORT
#if PK_LOGGER_WARNING_SUPPORT
    void _logWarningToFile(const char* msg, const char* sender)
    {
        if (isFileEnabled && isWarningEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[WARNING][Failed to write this message to log file](" << sender << "): " << msg << std::endl;
                return;
            }

            logFile << "[WARNING](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[WARNING][Failed to close log file after writing message](" << sender << "): " << msg << std::endl;
            }
        }
    }

    void _logWarningToFile(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isFileEnabled && isWarningEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[WARNING in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to write this message to log file]: " << msg << std::endl;
                return;
            }

            logFile << "[WARNING in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[WARNING in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to close log file after writing message]: " << msg << std::endl;
            }
        }
    }
#endif // PK_LOGGER_WARNING_SUPPORT
#if PK_LOGGER_INFO_SUPPORT
    void _logInfoToFile(const char* msg, const char* sender)
    {
        if (isFileEnabled && isInfoEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[INFO][Failed to write this message to log file](" << sender << "): " << msg << std::endl;
                return;
            }

            logFile << "[INFO](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[INFO][Failed to close log file after writing message](" << sender << "): " << msg << std::endl;
            }
        }
    }

    void _logInfoToFile(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isFileEnabled && isInfoEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[INFO in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to write this message to log file]: " << msg << std::endl;
                return;
            }

            logFile << "[INFO in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[INFO in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to close log file after writing message]: " << msg << std::endl;
            }
        }
    }
#endif // PK_LOGGER_INFO_SUPPORT
#if PK_LOGGER_DEBUG_SUPPORT
    void _logDebugToFile(const char* msg, const char* sender)
    {
        if (isFileEnabled && isDebugEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[DEBUG][Failed to write this message to log file](" << sender << "): " << msg << std::endl;
                return;
            }

            logFile << "[DEBUG](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[DEBUG][Failed to close log file after writing message](" << sender << "): " << msg << std::endl;
            }
        }
    }

    void _logAssertToConsole(const char* msg, const char* sender, const char* condition)
    {
        if (isConsoleEnabled)
        {
            std::cout << "(" << sender << "): " << "Assertion failed: " << condition << std::endl;
            const int spacesCount = std::strlen(sender) + 4;
            std::cout << SPACES_STR(spacesCount) << "with message \"" << msg << "\"" << std::endl;
        }
    }

    void _logAssertToConsole(const char* condition)
    {
        if (isConsoleEnabled)
        {
            std::cout << "Assertion failed: " << condition << std::endl;
        }
    }

    void _logDebugToFile(const char* msg, const char* sender, std::string_view sourceFileName, int sourceFileLine)
    {
        if (isFileEnabled && isDebugEnabled)
        {
            std::ofstream logFile(logFilePath, std::ios_base::app);
            if (!logFile.is_open())
            {
                std::cout << "[DEBUG in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to write this message to log file]: " << msg << std::endl;
                return;
            }

            logFile << "[DEBUG in " << sourceFileName << ":" << sourceFileLine << "](" << sender << "): " << msg << std::endl;

            logFile.close();
            if (logFile.is_open())
            {
                std::cout << "[DEBUG in " << sourceFileName << ":" << sourceFileLine << "](" << sender << ")[Failed to close log file after writing message]: " << msg << std::endl;
            }
        }
    }
#endif // PK_LOGGER_DEBUG_SUPPORT

#endif // PK_LOGGER_FILE_SUPPORT

#endif // PK_LOGGER_SUPPORT

} // namespace Logger
} // namespace Pekan
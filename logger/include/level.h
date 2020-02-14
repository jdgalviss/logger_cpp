#pragma once

enum class Level : unsigned int
{
    /// Mainly useful to represent current progress of application
    Info = 1,
    /// Informational events most useful for developers to debug application
    Debug = 2,
    /// @brief Useful when application has potentially harmful situtaions
    Warning = 3,
    /// @brief Information representing errors in application but application will keep running
    Error = 4,
    /// @brief Severe error information that will presumably abort application
    Fatal = 5,
};

class LevelHelper
{
    inline static const char *ConvertToString(Level level)
    {
        // Do not use switch over strongly typed enums because Intel C++ compilers dont support them yet.
        
        if (level == Level::Debug)
            return "DEBUG";
        if (level == Level::Info)
            return "INFO";
        if (level == Level::Warning)
            return "WARNING";
        if (level == Level::Error)
            return "ERROR";
        if (level == Level::Fatal)
            return "FATAL";
        return "UNKNOWN";
    }
};
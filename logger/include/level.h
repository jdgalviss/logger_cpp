/*
By: Juan David Galvis
jdgalviss@gmail.com
All rights reserved

@brief This class defines different levels of severity for log
messages
*/

#pragma once

enum class Level : unsigned int
{
    /// Mainly useful to represent current progress of application
    Info = 1,
    /// Informational events most useful for developers to debug application
    Debug = 2,
    /// Useful when application has potentially harmful situtaions
    Warning = 3,
    /// Information representing errors in application but application will keep running
    Error = 4,
    /// Severe error information that will presumably abort application
    Fatal = 5,
};

/*
@brief This class defines helper functions to deal with severity levels,
e.g. Convert level to string.
*/
class LevelHelper
{
    public:
    inline static const char *ConvertToString(Level level)
    {
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
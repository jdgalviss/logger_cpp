#pragma once
#include "configuration.h"
#include "level.h"
#define FORMAT_SPECIFIER_CHARACTER '%'
#define VALUE_SPECIFIER_CHARACTER 'v'


namespace cr
{
class Logger
{
public:
    Logger(std::string configuration_file_name);
#define LOGGER_LEVEL_DEFINITIONS(FUNCTION_NAME)         \
    template <typename T>             \
    inline void FUNCTION_NAME(const char *, const T &); \
    template <typename T>                               \
    inline void FUNCTION_NAME(const T &);               \
    inline void FUNCTION_NAME(const char *);

    LOGGER_LEVEL_DEFINITIONS(Info)
    LOGGER_LEVEL_DEFINITIONS(Debug)
    LOGGER_LEVEL_DEFINITIONS(Warning)
    LOGGER_LEVEL_DEFINITIONS(Error)
    LOGGER_LEVEL_DEFINITIONS(Fatal)
#undef LOGGER_LEVEL_DEFINITIONS

private:
    //void Logger::log_(Level level, int vlevel, const char* s, const T& value, const Args&... args) {
    template <typename T>
    void Log(Level, const char *, const T &);

    template <typename T>
    void Log(Level, const T &);

    void Log(Level, const char *);

        //void LogToTerminal(Level level, std::string message);
        //void LogToFile(std::string message);

        cr::Configuration *configuration_;
};
void PrintMessage();
#define LOGGER_LEVEL(FUNCTION_NAME, LOG_LEVEL)                         \
    template <typename T>                                              \
    inline void Logger::FUNCTION_NAME(const char *msg, const T &value) \
    {                                                                  \
        Log(LOG_LEVEL, msg, value);                        \
    }                                                                  \
    template <typename T>                                              \
    inline void Logger::FUNCTION_NAME(const T &value)                  \
    {                                                                  \
        Log(LOG_LEVEL, value);                                         \
    }                                                                  \
    inline void Logger::FUNCTION_NAME(const char *msg)                 \
    {                                                                  \
        Log(LOG_LEVEL, msg);                                           \
    }

LOGGER_LEVEL(Info, Level::Info);
LOGGER_LEVEL(Debug, Level::Debug);
LOGGER_LEVEL(Warning, Level::Warning);
LOGGER_LEVEL(Error, Level::Error);
LOGGER_LEVEL(Fatal, Level::Fatal);
} // namespace cr

#pragma once
#include "configuration.h"
#include "level.h"
#include <ctime>
#include <sstream>

#define FORMAT_SPECIFIER_CHARACTER '%'
#define VALUE_SPECIFIER_CHARACTER 'v'

namespace cr
{
class Logger
{
public:
    Logger(std::string configuration_file_name);
#define LOGGER_LEVEL_DEFINITIONS(FUNCTION_NAME)         \
    template <typename T>                               \
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
    template <typename T>
    void Log(Level, const char *, const T &);
    const void LogTerminal(Level, const char*);
    inline std::string GetLogHeader(Level);
    Configuration *configuration_;
    int log_count_ = 1;
};

#define LOGGER_LEVEL(FUNCTION_NAME, LOG_LEVEL)                         \
    template <typename T>                                              \
    inline void Logger::FUNCTION_NAME(const char *msg, const T &value) \
    {                                                                  \
        Log(LOG_LEVEL, msg, value);                                    \
    }                                                                  \
    template <typename T>                                              \
    inline void Logger::FUNCTION_NAME(const T &value)                  \
    {                                                                  \
        Log(LOG_LEVEL, "%v", value);                                         \
    }                                                                  \
    inline void Logger::FUNCTION_NAME(const char *msg)                 \
    {                                                                  \
        Log(LOG_LEVEL, msg, 0);                                           \
    }
LOGGER_LEVEL(Info, Level::Info);
LOGGER_LEVEL(Debug, Level::Debug);
LOGGER_LEVEL(Warning, Level::Warning);
LOGGER_LEVEL(Error, Level::Error);
LOGGER_LEVEL(Fatal, Level::Fatal);

inline std::string Logger::GetLogHeader(Level level){
    // current date/time based on current system
    time_t now = time(0);
    // convert now to string form
    char *t = ctime(&now);
    std::string time_now(t);
    const char *level_string = LevelHelper::ConvertToString(level);
    std::string log_header(level_string);
    log_header = std::to_string(log_count_) + " [" + log_header + "]: " + time_now.substr( 0, time_now.length() -1  ) + ": ";
    return log_header;
}



template <typename T>
void Logger::Log(Level level, const char *msg, const T &value)
{
    std::string log_msg;
    std::stringstream ss;
    ss << GetLogHeader(level);
    while (*msg){
        if (*msg == FORMAT_SPECIFIER_CHARACTER){
            msg++;
            if (*msg == VALUE_SPECIFIER_CHARACTER){
                ss << value;
                msg++;
            }
        }
        else{
            log_msg = log_msg + *msg;
            ss << *msg;
            msg++;
        }
    }
    log_msg = ss.str();
    if(configuration_->IsToFile())
        std::cout<<"nonono sisisi";

    if(configuration_->IsToTerminal())
        LogTerminal(level, log_msg.c_str());

}

} // namespace cr

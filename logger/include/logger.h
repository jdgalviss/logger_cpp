/*
By: Juan David Galvis
jdgalviss@gmail.com
All rights reserved

@brief This class defines members to perform thread safe, multilevel
logging both to files or to terminal
*/

#pragma once
#include "configuration.h"
#include "level.h"
#include "color_modifier.h"
#include <ctime>
#include <fstream>
#include <mutex>

#define FORMAT_SPECIFIER_CHARACTER '%'              //Characters used to specify when 
#define VALUE_SPECIFIER_CHARACTER 'v'               //the user wants to add a variable to the log
#define CONFIG_DEFAULT_PATH std::string(__FILE__).substr( 0, std::string(__FILE__).length() -8  ) + "../config/test.conf" //Careful: depends on current file name   


namespace cr
{
    typedef Color::Modifier ColorModifier;
    class Logger
    {
    public:
        static int log_count_;                          //member used to count total number of messages logged
        Logger(std::string configuration_file_name = CONFIG_DEFAULT_PATH);
        inline std::string GetLogFile(){return configuration_->getLogFileName();}

    //Macro used to create the definitions for the log functions for each level
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
        void Log(Level, const char *, const T &);       //Log member function (uses template to add variable to the log, e.g. int, string, float)
        bool LogTerminal (Level, const char*) const;    //Log to terminal
        bool LogFile (const char*) const;               //Log to file
        ColorModifier GetColor(Level) const;                  //Get Color of text depending on level
        inline std::string GetLogHeader(Level);         //Prewrite the log header with log number, severity and date.
        Configuration *configuration_;                  //Configuration from .config file
        std::mutex mtx_;                                // mutex to guarantee multithreading
    };

    //definition of inline logging functions for each severity level (this way logging can be called as follows: logger_ptr->Info(...), logger_ptr->Error,...)
    #define LOGGER_LEVEL(FUNCTION_NAME, LOG_LEVEL)                         \
        template <typename T>                                              \
        inline void Logger::FUNCTION_NAME(const char *msg, const T &value) \
        {                                                                  \
            Log(LOG_LEVEL, msg, value);                                    \
        }                                                                  \
        template <typename T>                                              \
        inline void Logger::FUNCTION_NAME(const T &value)                  \
        {                                                                  \
            Log(LOG_LEVEL, "%v", value);                                   \
        }                                                                  \
        inline void Logger::FUNCTION_NAME(const char *msg)                 \
        {                                                                  \
            Log(LOG_LEVEL, msg, 0);                                        \
        }
    LOGGER_LEVEL(Info, Level::Info);
    LOGGER_LEVEL(Debug, Level::Debug);
    LOGGER_LEVEL(Warning, Level::Warning);
    LOGGER_LEVEL(Error, Level::Error);
    LOGGER_LEVEL(Fatal, Level::Fatal);

    inline std::string Logger::GetLogHeader(Level level){
        time_t now = time(0);                           // current date/time based on current system                 
        char *t = ctime(&now);                          // convert now to string form
        std::string time_now(t);
        const char *level_string = LevelHelper::ConvertToString(level);
        std::string log_header(level_string);           //Create header
        log_header = std::to_string(log_count_) + " [" + log_header + "]: " + time_now.substr( 0, time_now.length() -1  ) + ":";
        return log_header;
    }



    template <typename T>
    void Logger::Log(Level level, const char *msg, const T &value)
    {
        mtx_.lock();                                    //Lock for current thread
        std::string log_msg;
        std::stringstream ss;
        ss << GetLogHeader(level);
        while (*msg){
            if (*msg == FORMAT_SPECIFIER_CHARACTER){    //Check if character correspond to format specifier
                msg++;
                if (*msg == VALUE_SPECIFIER_CHARACTER){ //Check if character correspond to value specifier
                    ss << value;                        //Add value to String stream
                    msg++;
                }
            }
            else{
                ss << *msg;                             //Add each character of the message
                msg++;
            }
        }
        log_msg = ss.str();
        bool log_success = true;
        if(configuration_->IsToFile())                  //check if log should be written to file
            if(!LogFile(log_msg.c_str())) log_success = false;
                
        if(configuration_->IsToTerminal())              //check if log should be written to terminal
            if(!LogTerminal(level, log_msg.c_str()) ) log_success = false;
                
        if(log_success)
            log_count_++;                               //if log was successfull, add count
        mtx_.unlock();                                  //Unlock for other threads
    }
} // namespace cr

/*
By: Juan David Galvis
jdgalviss@gmail.com
All rights reserved

@brief This header defines classes and members to perform thread safe, multilevel
logging both to files or to terminal
*/

#pragma once
#include <ctime>
#include <fstream>
#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp> 
#include <vector>
#include <ostream>

#define CONFIG_LOG_PATH "LogFilePath"                   //Name of the configuration in the .config file to define where logs will be stored                               
#define CONFIG_LOG_TO_FILE "LogToFile"                  //Name of the configuration in the .config file to define if logs will be saved in file                    
#define CONFIG_LOG_TO_TERMINAL "LogToTerminal"          //Name of the configuration in the .config file to define if logs will be shown in terminal
#define LOG_TO_FILE_DEFAULT false                       //Logs saved to file by default?
#define LOG_TO_TERMINAL_DEFAULT true                    //Logs printed in terminal by default?
#define CONFIG_TRUE "true"                              //How true must be written in the .config file
#define CONFIG_ASSERT_HEADER "[Logger Configuration]: " //Header for internal configuration logs
#define CONFIG_INTERNAL_LOG_ENABLE true                 //Enable internal logs?
#define LOG_FILE_DEFAULT std::string(__FILE__).substr( 0, std::string(__FILE__).length() -8  ) + "../log/default.log"  //Careful: depends on current file name        
//#define LOG_FILE_DEFAULT "Undefined"            

#if (CONFIG_INTERNAL_LOG_ENABLE)                        //If internal logs enabled, define macros
#define CONFIG_ASSERT(expr, msg) if (!(expr)) { \
std::stringstream internalAssertStream; internalAssertStream << CONFIG_ASSERT_HEADER << msg; \
std::cout << internalAssertStream.str(); } 
#define CONFIG_LOG(msg) \
std::stringstream internalInfoStream; internalInfoStream << CONFIG_ASSERT_HEADER << msg; \
std::cout << internalInfoStream.str();
#else 
#define CONFIG_ASSERT(expr, msg) 
#define CONFIG_LOG(msg) 
#endif

#define FORMAT_SPECIFIER_CHARACTER '%'              //Characters used to specify when 
#define VALUE_SPECIFIER_CHARACTER 'v'               //the user wants to add a variable to the log
#define CONFIG_DEFAULT_PATH std::string(__FILE__).substr( 0, std::string(__FILE__).length() -8  ) + "../config/config.conf" //Careful: depends on current file name   

/*
@brief This class defines different levels of severity for log
messages
*/

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

/*
@brief This class defines a operator<< in order to
add color to log msgs depending on their level
*/
namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {            
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        const char* ModifyColor(Level, const char*);
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }

    };
}
namespace cr
{
    typedef Color::Modifier ColorModifier;

    /*
    @brief This class defines members used to get the logger
    configuration from a .config file
    */
    class Configuration{
        public:
            Configuration();                            
            bool ParseFile(const std::string&);     //Function used to get configuration from file
            bool IsToFile();                        //Returns if logs should be written to file
            bool IsToTerminal();                    //Returns if logs should be printed in terminal
            std::string getLogFileName();           //Returns the name of the file where logs should be stored

        private:
            bool parseLine(std::string*);      //Get configuration defined in each line of the .config file
            std::string log_path_ = LOG_FILE_DEFAULT;                  //Path to the file where log files will be saved, if log_to_file_ is set to true
            bool log_to_file_ = false;
            bool log_to_terminal_ = true;
    };
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


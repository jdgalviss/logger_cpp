#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp> 
#include <vector>

#define CONFIG_LOG_PATH "LogFilePath"                   //Name of the configuration in the .config file to define where logs will be stored                               
#define CONFIG_LOG_TO_FILE "LogToFile"                  //Name of the configuration in the .config file to define if logs will be saved in file                    
#define CONFIG_LOG_TO_TERMINAL "LogToTerminal"          //Name of the configuration in the .config file to define if logs will be shown in terminal
#define LOG_TO_FILE_DEFAULT false                       //Logs saved to file by default?
#define LOG_TO_TERMINAL_DEFAULT true                    //Logs printed in terminal by default?
#define CONFIG_TRUE "true"                              //How true must be written in the .config file
#define CONFIG_ASSERT_HEADER "[Logger Configuration]: " //Header for internal configuration logs
#define CONFIG_INTERNAL_LOG_ENABLE true                 //Enable internal logs?

#if (CONFIG_INTERNAL_LOG_ENABLE) 
#define CONFIG_ASSERT(expr, msg) if (!(expr)) { \
std::stringstream internalInfoStream; internalInfoStream << CONFIG_ASSERT_HEADER << msg; \
std::cout << internalInfoStream.str(); } 
#define CONFIG_LOG(msg) \
std::stringstream internalInfoStream; internalInfoStream << CONFIG_ASSERT_HEADER << msg; \
std::cout << internalInfoStream.str();
#else 
#define CONFIG_ASSERT(expr, msg) 
#define CONFIG_LOG(msg) 
#endif

namespace cr {
    class Configuration{
        public:
            Configuration();
            bool ParseFile(const std::string& configuration_file);
            bool IsToFile();
            bool IsToTerminal();
            std::string getLogFileName();

        private:
            bool parseLine(std::string *line);

            std::string log_path_;                  //Path to the file where log files will be saved, if log_to_file_ is set to true
            bool log_to_file_ = false;
            bool log_to_terminal_ = true;
    };
}
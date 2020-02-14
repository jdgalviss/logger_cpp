#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp> 
#include <vector>

#define CONFIG_LOG_PATH "LogFilePath"
#define CONFIG_LOG_TO_FILE "LogToFile"
#define CONFIG_LOG_TO_TERMINAL "LogToTerminal"
#define CONFIG_TRUE "true"



namespace cr {
    class Configuration{
        public:
            Configuration();
            bool ParseFile(const std::string& configuration_file);

        private:
            bool IsComment(const std::string& line);
            bool parseLine(std::string *line);

            std::string log_path_;
            bool log_to_file_ = false;
            bool log_to_terminal_ = true;
    };
}
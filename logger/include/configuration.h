#pragma once

#include <string>
#include <iostream>
#include <fstream>

namespace logger {
    class Configuration{
        public:
            Configuration();
        
        private:
            bool ParseFile(const std::string& line);
            bool IsComment(const std::string& configuration_file);
            std::string log_directory_;
            bool log_to_file_ = false;
            bool log_to_terminal_ = true;
    };
}
#include "logger.h"

namespace cr
{
Logger::Logger(std::string configuration_file_name)
{
    configuration_ = new Configuration();
    configuration_->ParseFile(configuration_file_name);
}

template <typename T>
void Logger::Log(Level level, const char* msg, const T &value)
{
    const char* level_string = LevelHelper::ConvertToString(level);
    std::string log_msg;
    //Go through all msg
    while(*msg){
        if(*msg == FORMAT_SPECIFIER_CHARACTER){
            ++msg;
        }
        else
        {
            if(*msg == VALUE_SPECIFIER_CHARACTER){
                ++msg;
                //append value to string
            }
        }
    //append current character from msg to string
    ++msg;
    }
    //check if to terminal and if to file and send

    
}

template <typename T>
void Logger::Log(Level level, const T &value)
{
    
}

void Logger::Log(Level level, const char* msg)
{
    
}

void PrintMessage()
{
    std::cout << "Hello World" << std::endl;
    cr::Configuration *config = new Configuration();
    config->ParseFile("/home/jdgalviss/jdgalviss/testCRJapan/logger_cpp/test.conf");
}

} // namespace cr
#include "logger.h"

namespace cr
{
Logger::Logger(std::string configuration_file_name)
{
    configuration_ = new Configuration();
    configuration_->ParseFile(configuration_file_name);
}

bool Logger::LogTerminal (Level level, const char* log_msg) const{
    Color::Modifier red(Color::FG_RED);
    Color::Modifier green(Color::FG_GREEN);
    Color::Modifier yellow(Color::FG_BLUE);
    Color::Modifier def(Color::FG_DEFAULT);
    switch(level){
        case Level::Info:
        std::cout << def << log_msg << std::endl;
        break;

        case Level::Debug:
        std::cout << green << log_msg << std::endl;
        break;

        case Level::Warning:
        std::cout << yellow << log_msg << std::endl;
        break;

        case Level::Error:
        std::cout << red << log_msg << std::endl;
        break;

        case Level::Fatal:
        std::cout << red << log_msg << std::endl;
        break;
        default:
        std::cout << def << log_msg << std::endl;
        break;
    }
    return true;
}

bool Logger::LogFile (const char* log_msg) const{
    bool success = true;
    std::ofstream file;
    file.open (configuration_->getLogFileName(), std::fstream::in | std::fstream::out | std::fstream::app);
    if(file.is_open())
        file << log_msg << "\n";
    else
        success = false;
    file.close();
    return false;
}

} // namespace cr
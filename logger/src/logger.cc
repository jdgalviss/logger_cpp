#include "logger.h"
#include "color_modifier.h"

namespace cr
{
Logger::Logger(std::string configuration_file_name)
{
    configuration_ = new Configuration();
    configuration_->ParseFile(configuration_file_name);
}
const void Logger::LogTerminal(Level level, const char* log_msg){
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
}

} // namespace cr
#include "logger.h"

namespace cr
{
    Logger::Logger(std::string configuration_file_name){
        configuration_ = new Configuration();
        configuration_->ParseFile(configuration_file_name); //Open configuration file
    }

    ColorModifier Logger::GetColor(Level level) const{
        ColorModifier color(Color::FG_DEFAULT);
        switch (level){
        case Level::Info:
            color = ColorModifier(Color::FG_DEFAULT);
            break;

        case Level::Debug:
            color = ColorModifier(Color::FG_GREEN);
            break;

        case Level::Warning:
            color = ColorModifier(Color::FG_BLUE);
            break;

        case Level::Error:
            color = ColorModifier(Color::FG_RED);
            break;

        case Level::Fatal:
            color = ColorModifier(Color::FG_RED);
            break;
        }
        return color;
    }

    bool Logger::LogTerminal(Level level, const char *log_msg) const{
        Color::Modifier color = GetColor(level);
        std::cout << color << log_msg << std::endl;
        return true;
    }

    bool Logger::LogFile(const char *log_msg) const{
        bool success = true;
        std::ofstream file;
        file.open(configuration_->getLogFileName(), std::fstream::in | std::fstream::out | std::fstream::app);
        if (file.is_open())
            file << log_msg << "\n";
        else
            success = false;
        file.close();
        return success;
    }
} // namespace cr
#include "logger.h"

namespace cr
{
    Configuration::Configuration()
    {
        CONFIG_LOG("Trying to open configuration... \n");
    }

    bool Configuration::IsToFile(){
        return log_to_file_;
    }
    bool Configuration::IsToTerminal(){
        return log_to_terminal_;
    }

    std::string Configuration::getLogFileName(){
        return log_path_;
    }

    bool Configuration::ParseFile(const std::string &configuration_file)
    {
        bool success = false;
        std::ifstream file_stream(configuration_file.c_str(), std::ifstream::in);
        CONFIG_ASSERT(file_stream.is_open(), "Unable to open configuration file [" + configuration_file + "] for parsing. Using default configuration:\n"
                                            + CONFIG_ASSERT_HEADER + "logging to file: "  +  (LOG_TO_FILE_DEFAULT?"true \n":"false \n")
                                            + CONFIG_ASSERT_HEADER + "logging to terminal: " + (LOG_TO_TERMINAL_DEFAULT?"true \n":"false \n"));
        if(file_stream.is_open()){
            CONFIG_LOG("File[" + configuration_file + "] oppened for parsing.\n");
            std::string line = std::string();
            while (file_stream.good())
            {
                std::getline(file_stream, line);
                success = parseLine(&line);
                CONFIG_ASSERT(success, "Unable to parse configuration line: " + line + "\n");
            }
        }
        CONFIG_LOG("log directory: " + log_path_ + "\n"
                    + CONFIG_ASSERT_HEADER + std::string("logging to terminal: ") + (log_to_terminal_ ? "true\n" : "false\n")
                    + CONFIG_ASSERT_HEADER + std::string("logging to file: ")+ (log_to_file_ ? "true\n" : "false\n")
                    + CONFIG_ASSERT_HEADER + "Configuration complete \n \n");
    }

    bool Configuration::parseLine(std::string *line)
    {
        bool success = true;
        if (line->empty()) return true;             //Verify if line is empty
        if (line->at(0) == '#') return true;        //verify is line corresponds to comment
        line->erase(remove_if(line->begin(), line->end(), isspace), line->end()); //remove spaces from line

        //separate command from value
        std::vector<std::string> splitted_line;
        boost::split(splitted_line, *line, boost::is_any_of(":"));

        //Check the type of command
        if (splitted_line[0].compare(CONFIG_LOG_PATH) == 0)
        {
            log_path_ = splitted_line[1];
            // Check if file exists or can be openned
            std::ofstream file;
            file.open (log_path_, std::fstream::in | std::fstream::out | std::fstream::app);
            CONFIG_ASSERT(file.is_open(), "Unable to open Log file [" + log_path_ + "] Logs won't be saved.\n")
            file.close();
        }
        else if (splitted_line[0].compare(CONFIG_LOG_TO_FILE) == 0)
        {
            log_to_file_ = splitted_line[1].compare(CONFIG_TRUE) == 0;
        }
        else if (splitted_line[0].compare(CONFIG_LOG_TO_TERMINAL) == 0)
        {
            log_to_terminal_ = splitted_line[1].compare(CONFIG_TRUE) == 0;
        }
        else
        {
            success = false;
        }
        return success;
    }

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
#include "configuration.h"

namespace cr
{
Configuration::Configuration()
{
    std::cout << "Creating config..." << std::endl;
}

bool Configuration::IsToFile(){
    return log_to_file_;
}
bool Configuration::IsToTerminal(){
    return log_to_terminal_;
}

bool Configuration::ParseFile(const std::string &configuration_file)
{
    std::ifstream file_stream(configuration_file.c_str(), std::ifstream::in);
    //Assert
    //ELPP_ASSERT(file_stream.is_open(), "Unable to open configuration file [" + configuration_file + "] for parsing.");
    bool success = false;
    std::string line = std::string();
    while (file_stream.good())
    {
        std::getline(file_stream, line);
        success = parseLine(&line);
        //ELPP_ASSERT(success, "Unable to parse configuration line: " << line);
    }
}

bool Configuration::parseLine(std::string *line)
{
    bool success = true;
    //Verify if line is empty
    if (line->empty())
        return true;

    //verify is line corresponds to comment
    if (line->at(0) == '#')
        return true;

    //remove spaces from line
    line->erase(remove_if(line->begin(), line->end(), isspace), line->end());

    //std::cout<<*line<<std::endl;

    //separate command from value
    std::vector<std::string> splitted_line;
    boost::split(splitted_line, *line, boost::is_any_of(":"));
    //std::cout<<splitted_line[0]<<std::endl;
    //std::cout<<splitted_line[1]<<std::endl;
    std::cout << std::endl;
    //Check the type of command
    if (splitted_line[0].compare(CONFIG_LOG_PATH) == 0)
    {
        log_path_ = splitted_line[1];
        std::cout << "log directory: " << log_path_ << std::endl;
        //Assert directory exists
    }
    else if (splitted_line[0].compare(CONFIG_LOG_TO_FILE) == 0)
    {
        log_to_file_ = splitted_line[1].compare(CONFIG_TRUE) == 0;
        std::cout << "log to file? " << (log_to_file_ ? "true" : "false") << std::endl;
    }
    else if (splitted_line[0].compare(CONFIG_LOG_TO_TERMINAL) == 0)
    {
        log_to_terminal_ = splitted_line[1].compare(CONFIG_TRUE) == 0;
        std::cout << "log to terminal? " << (log_to_terminal_ ? "true" : "false") << std::endl;
    }
    else
    {
        success = false;
    }
}

}
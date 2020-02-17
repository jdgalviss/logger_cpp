/*
By: Juan David Galvis
jdgalviss@gmail.com
All rights reserved

@brief This class defines members used to get the logger
configuration from a .config file
*/
#include "configuration.h"

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
        CONFIG_LOG("Configuration complete \n");
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
            CONFIG_LOG("log directory: " + log_path_ + "\n");
            // Check if file exists or can be openned
            std::ofstream file;
            file.open (log_path_, std::fstream::in | std::fstream::out | std::fstream::app);
            CONFIG_ASSERT(file.is_open(), "Unable to open Log file [" + log_path_ + "] Logs won't be saved.\n")
            file.close();
        }
        else if (splitted_line[0].compare(CONFIG_LOG_TO_FILE) == 0)
        {
            log_to_file_ = splitted_line[1].compare(CONFIG_TRUE) == 0;
            CONFIG_LOG(std::string("logging to file: ")+ (log_to_file_ ? "true\n" : "false\n"));
        }
        else if (splitted_line[0].compare(CONFIG_LOG_TO_TERMINAL) == 0)
        {
            log_to_terminal_ = splitted_line[1].compare(CONFIG_TRUE) == 0;
            CONFIG_LOG(std::string("logging to terminal: ") + (log_to_terminal_ ? "true\n" : "false\n"));
        }
        else
        {
            success = false;
        }
        return success;
    }
} //namespace cr
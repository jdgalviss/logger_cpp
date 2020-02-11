#include "configuration.h"

namespace logger
{

bool Configuration::IsComment(const std::string &line)
{
}

bool Configuration::ParseFile(const std::string &configuration_file)
{
    std::ifstream file_stream(configuration_file.c_str(), std::ifstream::in);
    //Assert
    ELPP_ASSERT(file_stream.is_open(), "Unable to open configuration file [" << configurationFile << "] for parsing.");
    bool success = false;
    std::string line = std::string();
    while (file_stream.good())
    {
        std::getline(file_stream, line);
        parsedSuccessfully = parseLine(&line, &currConfigStr, &currLevelStr, &currLevel, sender);
        ELPP_ASSERT(parsedSuccessfully, "Unable to parse configuration line: " << line);
    }
}

} // namespace logger
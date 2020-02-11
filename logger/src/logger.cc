#include "logger.h"

namespace logger{
    void PrintMessage()
    {
        std::cout << "Hello World"<<std::endl;
        Configuration *config = new Configuration();
        config->ParseFile("/home/jdgalviss/jdgalviss/testCRJapan/logger_cpp/test.conf");

    }
}

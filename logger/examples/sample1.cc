#include "logger.h"

int cr::Logger::log_count_ = 1;

int main(){
    cr::Logger * logger = new cr::Logger("/home/jdgalviss/logger_cpp/logger/config/config.conf");
    std::string variable = "Something";
    logger->Info("This is an Info example", variable);
    logger->Warning("This is a Warning example");
}
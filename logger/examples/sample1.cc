#include "logger.h"

int cr::Logger::log_count_ = 1;     //Initialize the log count

int main(){
    cr::Logger * logger = new cr::Logger(); //Logger object
    std::string variable = "Info";
    logger->Info("This is an %v example", variable);
    logger->Warning("This is a Warning example");
    logger->Debug("This is the number %v Debug example", 1);
    logger->Error("This is an Error example");
}
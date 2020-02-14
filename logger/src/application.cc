#include "logger.h"
//home/kiwibot/jdgalviss/cr_test/logger_cpp
//home/jdgalviss/jdgalviss/testCRJapan/logger_cpp/test.conf
int main(){
    cr::Logger * logger = new cr::Logger("/home/kiwibot/jdgalviss/cr_test/logger_cpp/test.conf");
    std::string variable = "si";
    logger->Error("Hola %v Mundo", variable);
}
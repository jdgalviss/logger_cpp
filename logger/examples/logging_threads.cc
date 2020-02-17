#include "logger.h"
#include <thread>

int cr::Logger::log_count_ = 1;
cr::Logger * logger = new cr::Logger("/home/jdgalviss/logger_cpp/logger/config/config.conf");

void PrintLog(const char* msg){
    for(int i =0;i<5;i++){
        logger->Error(msg);
    }
}

int main(){
    std::thread th1 (PrintLog,"Logging from thread1");
    std::thread th2 (PrintLog,"Logging from thread2");
    th1.join();
    th2.join();
}
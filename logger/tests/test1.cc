/*
By: Juan David Galvis
jdgalviss@gmail.com
All rights reserved

@brief Automated tests for the logger Library
*/
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "logger.h"
#include "catch.hpp"
#include <thread>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#define TEST_LINES_TO_WRITE 50
#define TEST_DEFAULT_PATH std::string(__FILE__).substr( 0, std::string(__FILE__).length() -8  ) + "../config/test.conf" //Careful: depends on current file name   


int cr::Logger::log_count_ = 1;
cr::Logger *logger = new cr::Logger(TEST_DEFAULT_PATH);
std::string log_file = logger->GetLogFile();

int getLineId(std::string *line)
{
    if (line->empty())
        return -1; //Verify if line is empty
    //separate command from value
    std::vector<std::string> splitted_line;
    boost::split(splitted_line, *line, boost::is_any_of(" "));
    return std::stoi(splitted_line[0]);
}

std::string getMessage(std::string *line, std::string separator = ":", int index_to_return = 4 )
{
    if (line->empty())
        return ""; //Verify if line is empty
    //separate command from value
    std::vector<std::string> splitted_line;
    boost::split(splitted_line, *line, boost::is_any_of(separator));
    return splitted_line[index_to_return];
}

void WriteLogs(const char *msg)
{
    for (int i = 0; i < TEST_LINES_TO_WRITE; i++)
    {
        logger->Info(msg);
    }
}

TEST_CASE("Logging a msg", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is a test log";
    logger->Info(test_msg.c_str());
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE(test_msg.compare(getMessage(&line)) == 0);
    }
}

TEST_CASE("Logging a msg with a string variable", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is %v log";
    std::string variable= "a test";
    std::string final_test_msg= "This is a test log";
    logger->Info(test_msg.c_str(), variable);
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE(final_test_msg.compare(getMessage(&line)) == 0);
    }
}

TEST_CASE("Logging a msg with an int variable", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is %v log";
    int variable= 1;
    std::string final_test_msg= "This is 1 log";
    logger->Info(test_msg.c_str(), variable);
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE(final_test_msg.compare(getMessage(&line)) == 0);
    }
}

TEST_CASE("Info Logging", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is an info log";
    logger->Info(test_msg.c_str());
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE((getMessage(&line, " ", 1)).compare("[INFO]:") == 0);
    }
}

TEST_CASE("Debug Logging", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is a debug log";
    logger->Debug(test_msg.c_str());
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE((getMessage(&line, " ", 1)).compare("[DEBUG]:") == 0);
    }
}

TEST_CASE("Warning Logging", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is a warning log";
    logger->Warning(test_msg.c_str());
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE((getMessage(&line, " ", 1)).compare("[WARNING]:") == 0);
    }
}

TEST_CASE("Error Logging", "[Logger]"){
    //clean file
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    //Write a msg
    std::string test_msg= "This is an error log";
    logger->Error(test_msg.c_str());
    //Check if msg was correctly written to file
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    if (file_stream.is_open())
    {
        std::string line = std::string();
        std::getline(file_stream, line);
        REQUIRE((getMessage(&line, " ", 1)).compare("[ERROR]:") == 0);
    }
}


TEST_CASE("If there is messages coming from several threads, all should be logged, and the log_count (first value of the log) should keep its integrity as a counter", "[Logger]")
{
    //Write to file:
    logger->log_count_ = 1;
    std::ofstream file;
    file.open(log_file.c_str());
    file.close();
    std::thread th1(WriteLogs, "This is a test on thread 1");
    std::thread th2(WriteLogs, "This is a test on thread 2");
    std::thread th3(WriteLogs, "This is a test on thread 3");
    std::thread th4(WriteLogs, "This is a test on thread 4");
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    //Check if all lines were written
    std::ifstream file_stream(log_file.c_str(), std::ifstream::in);
    SECTION("File containing the logs is openned on path: " + log_file)
    {
        REQUIRE(file_stream.is_open());
    }
    if (file_stream.is_open())
    {
        std::string line = std::string();
        SECTION("Logs are saved to file or written to console, they contain a consecutive id. Ids should be consecutive, even when there is several threads logging msgs")
        {
            int expected_log_id = 1;
            while (file_stream.good())
            {
                std::getline(file_stream, line);
                int log_id = getLineId(&line);

                if (log_id != -1)
                {
                    REQUIRE(log_id == expected_log_id);
                    expected_log_id++;
                }
            }
            REQUIRE((expected_log_id - 1) == TEST_LINES_TO_WRITE * 4);
        }
        file_stream.close();
    }
}
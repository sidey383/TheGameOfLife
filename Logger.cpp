#include "Logger.h"
#include <iostream>
#include <regex>

bool Logger::isDebug = false;

Logger::Logger(std::string name): name(name) {}

static std::string buildOutString(std::string format, std::string name, std::tm time, std::string message) {
    std::string result = std::regex_replace(format, std::regex("%name"), name);
    result = std::regex_replace(result, std::regex("%message"), message);
    result = std::regex_replace(result, std::regex("%h"), std::to_string(time.tm_hour));
    result = std::regex_replace(result, std::regex("%m"), std::to_string(time.tm_min));
    result = std::regex_replace(result, std::regex("%s"), std::to_string(time.tm_sec));
    return result;
}

void Logger::info(std::string message) {
    std::tm now{};
    std::cout << buildOutString(infoFormat, name, now, message) << std::endl;
}

void Logger::error(std::string message) {
    std::tm now{};
    std::cerr << buildOutString(errorFormat, name, now, message) << std::endl;
}

void Logger::error(std::exception exception) {
    std::tm now{};
    std::cerr << buildOutString(errorFormat, name, now, exception.what())<< std::endl;
}

void Logger::setDebug(bool isDebug) {
    Logger::isDebug = isDebug;
}

void Logger::debug(std::string message) {
    if(isDebug) {
        std::tm now{};
        std::cout << buildOutString(debugFormat, name, now, message) << std::endl;
    }
}



#include "Logger.h"
#include <regex>
#include <ctime>
#include <utility>
#include <ostream>
#include <iostream>

Logger::LogLevel Logger::level = LogLevel::NoLog;


Logger::Logger(std::string name): name(std::move(name)) {}

void Logger::setLevel(Logger::LogLevel level) {
    Logger::level = level;
}

Logger::LogLevel Logger::getLevel() {
    return Logger::level;
}

Logger::LoggerStream Logger::info() const {
    return {this->name, infoFormat, std::clog, (LogLevel::Info >= this->level)};
}

Logger::LoggerStream Logger::error() const {
    return {this->name, errorFormat, std::cerr, (LogLevel::Error >= this->level)};
}

Logger::LoggerStream Logger::debug() const {
    return {this->name, debugFormat, std::clog, (LogLevel::Debug >= this->level)};
}

static std::string buildOutString(std::string format, std::string name) {
    std::time_t t = std::time(nullptr);
    std::tm* time = std::localtime(&t);
    std::string result = std::regex_replace(format, std::regex("%name"), name);
    result = std::regex_replace(result, std::regex("%h"), std::to_string(time->tm_hour));
    result = std::regex_replace(result, std::regex("%m"), std::to_string(time->tm_min));
    result = std::regex_replace(result, std::regex("%s"), std::to_string(time->tm_sec));
    return result;
}


Logger::LoggerStream::LoggerStream(std::string name, std::string format, std::ostream& stream, bool isEnabled):
name(std::move(name)), stream(stream), format(std::move(format)), isEnabled(isEnabled) {
    if (isEnabled) {
        stream << buildOutString(this->format, this->name);
    }
}

Logger::LoggerStream const & Logger::LoggerStream::operator<<(const std::string &str) const {
    if (isEnabled)
        this->stream << str;
    return *this;
}

Logger::LoggerStream const & Logger::LoggerStream::operator<<(const char* str) const {
    if (isEnabled)
        this->stream << str;
    return *this;
}

Logger::LoggerStream const & Logger::LoggerStream::operator<<(int val) const {
    if (isEnabled)
        this->stream << val;
    return *this;
}

Logger::LoggerStream const & Logger::LoggerStream::operator<<(unsigned int val) const {
    if (isEnabled)
        this->stream << val;
    return *this;
}

Logger::LoggerStream const & Logger::LoggerStream::operator<<(char ch) const {
    if (isEnabled)
        this->stream.put(ch);
    return *this;
}

const Logger::LoggerStream &Logger::LoggerStream::operator<<(const FileFormatException &exception) const {
    if (isEnabled) {
        this->stream << exception.getReason() << std::endl << "\t" << exception.getString() << std::endl;
        char errorPointer[exception.errorEnd() + 1];
        errorPointer[exception.errorEnd()] = 0;
        memset(errorPointer, ' ', exception.errorStart());
        memset(errorPointer + exception.errorStart(), '^', exception.errorEnd() - exception.errorStart());
        this->stream << "\t" << errorPointer;
    }
    return *this;
}

Logger::LoggerStream::~LoggerStream() {
    if(isEnabled)
        (stream<<std::endl).flush();
}

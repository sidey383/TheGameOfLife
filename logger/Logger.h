#pragma once
#include <string>
#include <ostream>
#include "../game/FileFormatException.h"

class Logger {
public:
    enum LogLevel {
        Debug = 0,
        Info = 1,
        Error = 2,
        NoLog = 3
    };
private:

    std::string infoFormat = "[%h:%m:%s] [Info/%name]: ";

    std::string errorFormat = "[%h:%m:%s] [Error/%name]: ";

    std::string debugFormat = "[%h:%m:%s] [Debug/%name]: ";

    std::string name;

    static LogLevel level;

public:

    explicit Logger(std::string name);

    static void setLevel(LogLevel level);

    static LogLevel getLevel();


    class LoggerStream {
        friend Logger;
        std::ostream &stream;
        std::string name;
        std::string format;
        bool isEnabled;

        LoggerStream(std::string name, std::string format, std::ostream &stream, bool isEnabled);

    public:

        ~LoggerStream();

        LoggerStream const & operator<<(std::string const &str) const;

        LoggerStream const & operator<<(char const *str) const;

        LoggerStream const & operator<<(int str) const;

        LoggerStream const & operator<<(unsigned int str) const;

        LoggerStream const & operator<<(char ch) const;

        LoggerStream const & operator<<(FileFormatException const &exception) const;

    };

    Logger::LoggerStream info() const;

    Logger::LoggerStream error() const;

    Logger::LoggerStream debug() const;

};

#pragma once
#include <string>

class Logger {
private:

    std::string format = "[%h:%m:%s] [%name]: %message";

    std::string name;

public:

    Logger(std::string name);

    void info(std::string message);

    void error(std::string message);

};

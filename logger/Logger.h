#pragma once
#include <string>

class Logger {
private:

    std::string infoFormat = "[%h:%m:%s] [Info/%name]: %message";

    std::string errorFormat = "[%h:%m:%s] [Error/%name]: %message";

    std::string debugFormat = "[%h:%m:%s] [Debug/%name]: %message";

    std::string name;

    static bool useDebug;

public:

    Logger(std::string name);

    void info(std::string message);

    void error(std::string message);

    void error(std::exception exception);

    void debug(std::string message);

    static void setDebug(bool isDebug);

    static bool isDebug();

};

#pragma once
#include <stdexcept>

class FileFormatException: public std::exception {

public:
    FileFormatException(std::string readString, );

};

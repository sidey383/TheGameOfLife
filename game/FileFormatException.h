#pragma once
#include <stdexcept>

class FileFormatException: public std::exception {
    std::string reason;
    std::string string;
    unsigned int errorS;
    unsigned int errorE;

public:
    FileFormatException(std::string reason, std::string string, unsigned int errorStart, unsigned int errorEnd);

    FileFormatException(std::string reason, char* string, unsigned int errorStart, unsigned int errorEnd);

    std::string getString() const;

    unsigned int errorStart() const;

    unsigned int errorEnd() const;

    std::string getReason() const;


};

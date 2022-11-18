#include "FileFormatException.h"

FileFormatException::FileFormatException(std::string reason, std::string string, unsigned int errorStart, unsigned int errorEnd):
    reason(std::move(reason)), string(std::move(string)), errorS(errorStart), errorE(errorEnd){}

FileFormatException::FileFormatException(std::string reason, char* string, unsigned int errorStart, unsigned int errorEnd):
        reason(std::move(reason)), string(string), errorS(errorStart), errorE(errorEnd){}


std::string FileFormatException::getString() const {
    return string;
}

unsigned int FileFormatException::errorStart() const {
    return errorS;
}

unsigned int FileFormatException::errorEnd() const {
    return errorE;
}

std::string FileFormatException::getReason() const {
    return reason;
}

#include "GameFieldIO.h"
#include "GameRules.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <cstring>

#define BUFFER_SIZE 2048

using namespace gol;

struct GameFieldLore {
    int width;
    int height;
    std::vector<std::pair<int, int>> dots;
    GameRules rules;
    std::string name;
};
const GameFieldLore defaultFields[]{
        {15, 15,
                {{5, 5}, {6, 5}, {7, 5}, {7, 4}, {6, 3}},
                GameRules(),
                "Glider"},
        {5,  5,
                {{3, 3}, {2, 3}, {4, 3}},
                GameRules(),
                "Blinkers"},
        {12, 12,
                {{6, 6}, {6, 7}, {7, 6}, {7, 7}},
                GameRules({2}, {2}),
                "Cube"}
};

GameFieldIO::GameFieldIO(std::string path) : path(std::move(path)) {}

std::pair<int, int> GameFieldIO::readNumbers(char *buffer) const {
    unsigned int size = strlen(buffer);
    std::pair<int, int> pair;
    bool hasFirst = false;
    bool completeFirst = false;
    bool hasSecond = false;
    unsigned int i = 0;
    for (; i < size; i++) {
        if (std::isdigit(buffer[i])) {
            if (completeFirst) {
                hasSecond = true;
                pair.second = pair.second * 10 + (buffer[i] - '0');
            } else {
                hasFirst = true;
                pair.first = pair.first * 10 + (buffer[i] - '0');
            }
        } else {
            if (!completeFirst) {
                if(!hasFirst)
                    break;
                completeFirst = true;
            } else {
                break;
            }
        }
    }
    if (!(hasFirst && hasSecond)) {
        throw FileFormatException("expected pair of natural numbers", buffer, i, i + 1);
    }
    if(buffer[i] != '\0') {
        logger.error() << FileFormatException("expected end of line", buffer, i, i+1);
    }
    return pair;
}

GameField GameFieldIO::readField() const {
    std::string name = path;
    bool noName = true;
    GameRules rules;
    bool noRules = true;
    std::pair<int, int> size(32, 32);
    bool noSize = true;

    std::vector<std::pair<int, int>> dots;

    std::ifstream input(path);
    if (input.fail()) {
        throw std::invalid_argument("invalid file");
    }
    char buffer[BUFFER_SIZE];
    if (!input.getline(buffer, BUFFER_SIZE) || std::string(buffer) != fileHeader) {
        throw FileFormatException("Wrong file header", buffer, 0, strlen(buffer));
    }
    while (input.getline(buffer, BUFFER_SIZE)) {
        try {
            if (buffer[0] == '#') {
                switch (buffer[1]) {
                    case 'N':
                        if (std::isspace(buffer[2])) {
                            name = std::string(buffer + 3);
                        } else {
                            name = std::string(buffer + 2);
                            logger.error() << FileFormatException("No space after #N", buffer, 2, 3);
                        }
                        if (noName) {
                            noName = false;
                        } else {
                            logger.error() << FileFormatException("Two names in one file", buffer, 0, strlen(buffer));
                        }

                        break;
                    case 'R':
                        if (std::isspace(buffer[2])) {
                            rules = GameRules(buffer + 3);
                        } else {
                            rules = GameRules(buffer + 2);
                            logger.error() << FileFormatException("No space after #R", buffer, 2, 3);
                        }
                        if (noRules) {
                            noRules = false;
                        } else {
                            logger.error() << FileFormatException("Two rules in one file", buffer, 0, strlen(buffer));
                        }
                        break;
                    case 'S':
                        if (std::isspace(buffer[2])) {
                            size = readNumbers(buffer + 3);
                        } else {
                            size = readNumbers(buffer + 2);
                            logger.error() << FileFormatException("No space after #S", buffer, 2, 3);
                        }
                        if (noSize) {
                            noSize = false;
                        } else {
                            logger.error()
                                    << FileFormatException("Two field size in one file", buffer, 0, strlen(buffer));
                        }
                        break;
                    default:
                        logger.error() << FileFormatException("Unknown value type", buffer, 0, strlen(buffer));
                        break;
                }
            } else {
                dots.push_back(readNumbers(buffer));
                logger.debug() << "read dots " << dots[dots.size() - 1].first << ":" << dots[dots.size() - 1].second;
            }
        } catch (FileFormatException &e) {
            logger.error() << "can't read line, " << e;
        }
    }

    bool data[size.first * size.second];

    for (int x = 0; x < size.first; x++) {
        for (int y = 0; y < size.second; y++) {
            data[GameField::getArrayPose(x, y, size.first, size.second)] = false;
        }
    }

    for (std::pair<int, int> dot: dots) {
        data[GameField::getArrayPose(dot.first, dot.second, size.first, size.second)] = true;
    }

    if (noRules)
        logger.error() << "No game rules, use default";
    if (noName)
        logger.error() << "No world name, use file name as name";
    if (noSize)
        logger.info() << "No field size, use field 32*32";

    input.close();
    return {rules, noName ? path : name, data, size.first, size.second};
}

void GameFieldIO::writeInFile(GameField &field) const {
    std::ofstream output(path);
    output << fileHeader << std::endl;
    output << "#N " << field.getName() << std::endl;
    output << "#R " << field.getRules() << std::endl;
    output << "#S " << field.getWidth() << " " << field.getHeight() << std::endl;
    for (int x = 0; x < field.getWidth(); x++) {
        for (int y = 0; y < field.getHeight(); y++) {
            if (field.getData(x, y)) {
                output << x << " " << y << std::endl;
            }
        }
    }
    output.close();
}

GameField GameFieldIO::getDefault(unsigned int number) {
    Logger log("DefaultGameFieldBuilder");
    number = number % (sizeof(defaultFields) / sizeof(defaultFields[0]));
    log.debug() << "chosen number " << number;
    GameFieldLore lore = defaultFields[number];
    bool data[lore.width * lore.height];
    for (int x = 0; x < lore.width; x++) {
        for (int y = 0; y < lore.height; y++) {
            data[GameField::getArrayPose(x, y, lore.width, lore.height)] = false;
        }
    }
    for (std::pair<int, int> dot: lore.dots) {
        log.debug() << "set dot " << dot.first << ":" << dot.second;
        data[GameField::getArrayPose(dot.first, dot.second, lore.width, lore.height)] = true;
    }
    return {lore.rules, lore.name, data, lore.width, lore.height};
}

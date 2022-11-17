#include "GameFieldIO.h"
#include "GameRules.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

#define BUFFER_SIZE 2048

using namespace gol;

struct GameFieldLore{
    int width;
    int height;
    std::vector<std::pair<int, int>> dots;
    GameRules rules;
    std::string name;
};

GameFieldLore defaultFields[]{
    {15, 15,
     {{5, 5}, {6,5}, {7,5}, {7,4}, {6,3}},
     GameRules(),
     "Glider"},
    {5, 5,
     {{3,3}, {2,3}, {4,3}},
     GameRules(),
     "Blinkers"}
};

GameFieldIO::GameFieldIO(std::string path): path(path) {}

std::pair<int, int> GameFieldIO::readNumbers(char *buffer) {
    std::stringstream readStream((std::string(buffer)));
    std::pair<int, int> pair;
    readStream >> pair.first >> pair.second;
    return pair;
}

GameField GameFieldIO::readField() {
    std::string name = path;
    bool noName = true;
    GameRules rules;
    bool noRules = true;
    std::pair<int, int> size(64, 64);
    bool noSize = true;

    std::vector<std::pair<int, int>> dots;

    std::ifstream input(path);
    char buffer[BUFFER_SIZE];
    if(!input.getline(buffer, BUFFER_SIZE) || buffer != fileHeader)
        throw std::invalid_argument("wrong file header");
    while(input.getline(buffer, BUFFER_SIZE)) {
        try {
            if (buffer[0] == '#') {
                switch (buffer[1]) {
                    case 'N':
                        if (std::isspace(buffer[2])) {
                            name = std::string(buffer + 3);
                        } else {
                            name = std::string(buffer + 2);
                            logger.error("No space after #N");
                        }
                        if (noName) {
                            noName = false;
                        } else {
                            logger.error("Two names in one file!");
                        }

                        break;
                    case 'R':
                        if (std::isspace(buffer[2])) {
                            rules = GameRules(buffer + 3);
                        } else {
                            rules = GameRules(buffer + 2 );
                            logger.error("No space after #R");
                        }
                        if (noRules) {
                            noRules = false;
                        } else {
                            logger.error("Two rules in one file!");
                        }
                        break;
                    case 'S':
                        if (std::isspace(buffer[2])) {
                            try {
                               size = readNumbers(buffer+3);
                            } catch (std::exception e) {
                                logger.error(e);
                            }
                        } else {
                            try {
                                size = readNumbers(buffer+2);
                            } catch (std::exception e) {
                                logger.error(e);
                            }
                            logger.error("No space after #S");
                        }
                        if (noSize) {
                            noSize = false;
                        } else {
                            logger.error("Two field size in one file!");
                        }
                        break;
                    default:
                        logger.error("Unknown argument type!");
                        break;
                }
            } else {
                dots.push_back(readNumbers(buffer));
            }
        } catch (std::exception e) {
            logger.error(e);
        }
    }

    bool data[size.first * size.second];

    for(std::pair<int, int> dot : dots) {
        data[GameField::getArrayPose(dot.first, dot.second, size.first, size.second)] = true;
    }

    if(noRules)
        logger.error("No game rules, use default");
    if(noName)
        logger.error("No world name, use file name as name");
    if(noSize)
        logger.info("No field size, use field 64*64");

    input.close();
    return GameField(rules, name, data, size.first, size.second);
}

void GameFieldIO::writeInFile(GameField& field) {
    std::ofstream output(path);
    output << fileHeader;
    output << "#N" << field.getName() << std::endl;
    output << "#R " << field.getRules() << std::endl;
    output << "#S " << field.getWidth() << " " << field.getHeight() << std::endl;
    for(int x = 0; x < field.getWidth(); x++) {
        for(int y = 0; y < field.getHeight(); y++) {
            if(field.getData(x, y)) {
                output << x << " " << y << std::endl;
            }
        }
    }
    output.close();
}

GameField GameFieldIO::getDefault() {
    int number = sizeof(defaultFields) / sizeof(GameFieldLore);
    number = rand() % number;
    GameFieldLore lore = defaultFields[number];
    bool data[lore.width*lore.height];
    Logger log("DefaultGameFieldBuilder");
    for (int x = 0; x < lore.width; x++) {
        for (int y = 0; y < lore.height; y++) {
            data[GameField::getArrayPose(x, y, lore.width, lore.height)] = false;
        }
    }
    for(std::pair<int, int> dot : lore.dots) {
        log.debug("set dot " + std::to_string(dot.first) + ":" + std::to_string(dot.second));
        data[GameField::getArrayPose(dot.first, dot.second, lore.width, lore.height)] = true;
    }
    return GameField(lore.rules, lore.name, data, lore.width, lore.height);
}

#include "GameOfLifeConsoleInterface.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace gol;

static void printHelp() {
    std::cout << "dump <filename> - save world in file" << std::endl
            << "tick <n=1> - tick world n times, default 1" << std::endl
            << "exit - exit from program" << std::endl
            << "help - show this text" << std::endl;
}

static void printField(GameField &field, int iteration) {
    std::cout << "Name: " << field.getName() << std::endl;
    std::cout << "Rules: " << field.getRules() << std::endl;
    std::cout << "Iteration: " << iteration << std::endl;
    std::cout << "Size: " << field.getWidth() << "x" << field.getHeight() << std::endl;

    for(int y = 0; y < field.getHeight(); y++) {
        for(int x = 0; x < field.getWidth(); x++) {
            std::cout<<(field.getData(x, y)?"*":" ");
        }
        std::cout<<std::endl;
    }
}

void GameOfLifeConsoleInterface::start() {
    printHelp();
    while(true) {
        char buffer[2048];
        logger.debug("wait for command");
        std::cin.getline(buffer, 2048);
        std::vector <std::string> args;
        std::string word;
        std::stringstream s(buffer);
        while (s >> word)
            args.push_back(word);
        if(args[0] == "exit") {
            break;
        }
        if(args[0] == "tick" ) {
            int n = 0;
            if(args.size() > 1) {
                try {
                    n = std::stoi(args[1]);
                } catch (std::exception &e) {
                    logger.error(e);
                }
                for(int i = 0; i < n; i++) {
                    field.tick();
                    iterationCount++;
                }
            } else {
                printHelp();
                continue;
            }
            printField(field, iterationCount);
            logger.debug("field printed");
            continue;
        }
        if(args[0] == "dump") {
            if(args.size() == 1) {
                printHelp();
                continue;
            }
            GameFieldIO io(args[1]);
            try {
                io.writeInFile(field);
            } catch (std::exception &e) {
                logger.error(e);
            }
            continue;
        }
        printHelp();
    }
}

GameOfLifeConsoleInterface::GameOfLifeConsoleInterface(GameField &field): field(field) {}

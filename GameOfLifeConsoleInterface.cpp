#include "GameOfLifeConsoleInterface.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace gol;

static void printHelp() {
    std::cout
    << "===============================================" << std::endl
    << "\tdump <filename> - save world in file" << std::endl
    << "\ttick <n=1> - tick world n times, default 1" << std::endl
    << "\texit - exit from program" << std::endl
    << "\thelp - show this text" << std::endl
    << "===============================================" << std::endl;
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
    printField(field, iterationCount);
    printHelp();
    while(true) {
        char buffer[2048];
        logger.debug() << "wait for command";
        std::cin.getline(buffer, 2048);
        std::vector <std::string> args;
        std::string word;
        std::stringstream s(buffer);
        while (s >> word)
            args.push_back(word);
        if(args[0] == "exit") {
            std::cout << "Bye!\n";
            break;
        }
        if(args[0] == "tick" ) {
            if(args.size() > 1) {
                int n = std::stoi(args[1]);
                for(int i = 0; i < n; i++) {
                    field.tick();
                    iterationCount++;
                }
            } else {
                printHelp();
                continue;
            }
            printField(field, iterationCount);
            logger.debug() << "Field printed";
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
                logger.error() << e.what();
                continue;
            }
            std::cout << "Write in file!\n";
            continue;
        }
        printHelp();
    }
}

GameOfLifeConsoleInterface::GameOfLifeConsoleInterface(GameField &field): field(field) {}

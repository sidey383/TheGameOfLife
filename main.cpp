#include <iostream>
#include "logger/Logger.h"
#include "GameOfLifeConsoleInterface.h"
#include <cstring>
#include <ctime>

struct InputState {
    char* outputFile;
    char* inputFile;
    int iterationCount;
};

static void printHelp() {
    std::cout << "GameOfLife help - to see this" << std::endl
            << "GameOfLife <file>" << std::endl
            << "Arguments:" << std::endl
            << "    -i <x> or --iterations=<x> - count of iterations." << std::endl
            << "    -in <filename> or --input=<file name> - input file." << std::endl
            << "    -o <filename> or --output=<filename> - output file." << std::endl
            << "When the output file is not specified open interactive mode." << std::endl
            << "When the input file is not specified generate on of default configuration." << std::endl;
}

static InputState readState(int size, char** args) {
    Logger logger("ParseArgs");
    InputState state {nullptr, nullptr, 0};
    for(int i = 1; i < size; i++) {
        try {
            if (!strcmp(args[i], "-i")) {
                if (i + 1 < size) {
                    state.iterationCount = std::stoi(args[i + 1]);
                    if(state.iterationCount <= 0) {
                        logger.error() << "count of iterations not natural number";
                    }
                } else {
                    logger.error() << "No value for key -i";
                }
            }

            if (!strcmp(args[i], "--iterations=")) {
                state.iterationCount = std::stoi(args[i] + 12);
                if(state.iterationCount <= 0) {
                    logger.error() << "count of iterations not natural number";
                }
            }
            if (!strcmp(args[i], "-o")) {
                if (i + 1 < size) {
                    state.outputFile = args[i+1];
                } else {
                    logger.error() << "No value for key -o";
                }
            }

            if (!strcmp(args[i], "--output=")) {
                state.outputFile = args[i] + 12;
            }
            if (!strcmp(args[i], "-in")) {
                if (i + 1 < size) {
                    state.inputFile = args[i+1];
                } else {
                    logger.error() << "No value for key -in";
                }
            }

            if (!strcmp(args[i], "--input=")) {
                state.inputFile = args[i] + 8;
            }
        } catch (std::exception &e) {
            logger.error() << e.what();
        }
    }
    return state;
}

int main(int argc, char** argv) {
    Logger::setLevel(Logger::Error);
    InputState state = readState(argc, argv);
    Logger logger("Main");
    logger.debug()
            << "input file: " << (!state.inputFile ? "null" : state.inputFile) << "\n"
            << "output file: " << (!state.outputFile ? "null" : state.outputFile) << "\n"
            << "iteration: " << std::to_string(state.iterationCount);

    gol::GameField field;
    if (state.inputFile != nullptr) {
        gol::GameFieldIO fieldIo(state.inputFile);
        try {
            field = fieldIo.readField();
        } catch (FileFormatException &e) {
            logger.error() << e;
        } catch (std::exception &e) {
            logger.error() << e.what();
        }
    } else {
        srand(time(nullptr));
        field = gol::GameFieldIO::getDefault(rand());
    }

    try {
        if (state.outputFile != nullptr && state.iterationCount > 0) {
            for (int i = 0; i < state.iterationCount; i++) {
                field.tick();
            }
            gol::GameFieldIO(state.outputFile).writeInFile(field);
        } else {
            gol::GameOfLifeConsoleInterface interface(field);
            interface.start();
        }
    } catch (FileFormatException &e) {
        logger.error() << e;
    } catch (std::exception &e) {
        logger.error() << e.what();
    }
    return 0;
}

#include <iostream>
#include "logger/Logger.h"
#include "GameOfLifeConsoleInterface.h"
#include <cstring>
#include <ctime>

struct InputState {
    char* outputFile;
    char* inputFile;
    int iterationCount;
    bool hasUnknown = false;
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
                if (++i < size) {
                    try {
                        state.iterationCount = std::stoi(args[i]);
                    } catch (const std::invalid_argument & e) {
                        logger.error() << "count of iterations not natural number";
                    } catch (const std::out_of_range & e) {
                        logger.error() << "count of iterations too large";
                    }
                } else {
                    logger.error() << "No value for key -i";
                }
                continue;
            }

            if (!strcmp(args[i], "--iterations=")) {
                state.iterationCount = std::stoi(args[i] + 12);
                if(state.iterationCount <= 0) {
                    logger.error() << "count of iterations not natural number";
                }
                continue;
            }
            if (!strcmp(args[i], "-o")) {
                if (++i < size) {
                    state.outputFile = args[i];
                } else {
                    logger.error() << "No value for key -o";
                }
                continue;
            }

            if (!strcmp(args[i], "--output=")) {
                state.outputFile = args[i] + 12;
                continue;
            }
            if (!strcmp(args[i], "-in")) {
                if (++i < size) {
                    state.inputFile = args[i];
                } else {
                    logger.error() << "No value for key -in";
                }
                continue;
            }

            if (!strcmp(args[i], "--input=")) {
                state.inputFile = args[i] + 8;
                continue;
            }
            state.hasUnknown = true;
        } catch (std::exception &e) {
            logger.error() << e.what();
        }
    }
    return state;
}

int main(int argc, char** argv) {
    Logger::setLevel(Logger::Error);
    InputState state = readState(argc, argv);
    if(state.hasUnknown) {
        printHelp();
        return 0;
    }
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

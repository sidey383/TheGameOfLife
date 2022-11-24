cmake_minimum_required(VERSION 3.20)
project(TheGameOfLife)

set(CMAKE_CXX_STANDARD 14)

add_executable(TheGameOfLife main.cpp game/GameField.cpp game/GameRules.cpp logger/Logger.cpp game/GameFieldIO.cpp GameOfLifeConsoleInterface.cpp game/FileFormatException.cpp)

add_executable(Tester test/Tester.cpp game/GameField.cpp game/GameRules.cpp logger/Logger.cpp game/GameFieldIO.cpp GameOfLifeConsoleInterface.cpp game/FileFormatException.cpp )

link_libraries(TheGameOfLife libstdc++-6.dll)

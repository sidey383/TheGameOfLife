cmake_minimum_required(VERSION 3.20)
project(TheGameOfLife)

set(CMAKE_CXX_STANDARD 14)

add_executable(TheGameOfLife main.cpp game/GameField.cpp game/GameRules.cpp logger/Logger.cpp game/GameFieldIO.cpp GameOfLifeConsoleInterface.cpp)

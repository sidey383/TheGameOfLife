cmake_minimum_required(VERSION 3.20)
project(TheGameOfLife)

set(CMAKE_CXX_STANDARD 14)

add_executable(TheGameOfLife main.cpp GameField.cpp GameRules.cpp Logger.cpp)

#include "../game/gameoflife.h"
#include "GameRulesTest.cpp"

int main(int argc, char **argv) {
    Logger::setLevel(Logger::NoLog);
    gameRulesTest();
    return 0;
}

#include "GameField.h"
#include <utility>

using namespace gol;

GameField::GameField(GameRules rules, std::string name, const bool* data, int width, int height):
        rules(std::move(rules)), name(std::move(name)), width(width), height(height) {
    this->data = (bool*) malloc(sizeof(bool) * width * height);
    memcpy(this->data, data, sizeof(bool) * width * height);
}

GameField::~GameField() {
    free(data);
}

unsigned int GameField::getArrayPose(int x, int y, int width, int height) {
    return (x > 0 ?
            (x % width) :
            width + (-1 +  (x % width)))
           + width *
             (y > 0 ?
              (y % height) :
              (height + (-1 + (y%height))));
}

unsigned int GameField::getArrayPose(int x, int y) {
    return getArrayPose(x, y, width, height);
}

bool GameField::getData(int x, int y) {
    return data[getArrayPose(x, y)];
}

int GameField::getHeight() {
    return height;
}

int GameField::getWidth() {
    return width;
}

GameRules GameField::getRules() {
    return rules;
}

void GameField::setDot(int x, int y, bool val) {
    data[getArrayPose(x, y)] = val;
}

void GameField::tick() {
    bool* data = (bool*) malloc(sizeof(bool) * width * height);
    for (int x = 0; x < width; x++) {
        int sum = this->data[getArrayPose(x-1, -1)]
                + this->data[getArrayPose(x, -1)]
                + this->data[getArrayPose(x+1, -1)]
                + this->data[getArrayPose(x-1, 1)]
                + this->data[getArrayPose(x, 1)]
                + this->data[getArrayPose(x+1, 1)]
                + this->data[getArrayPose(x-1, 0)]
                + this->data[getArrayPose(x+1, 0)];
        for (int y = 0; y < height; y++) {
            data[getArrayPose(x, y)] = rules.isActive(this->data[getArrayPose(x, y)], sum);
            /* how change state of sum?
             *  x
             * rrr
             * 0a0 y
             * 0r0
             * aaa
             * */
            sum += this->data[getArrayPose(x-1, y+2)]
                   + this->data[getArrayPose(x, y+2)]
                   + this->data[getArrayPose(x+1, y+2)]
                   + this->data[getArrayPose(x, y)];
            sum -= this->data[getArrayPose(x-1, y-1)]
                   + this->data[getArrayPose(x, y-1)]
                   + this->data[getArrayPose(x+1, y-1)]
                   + this->data[getArrayPose(x, y+1)];
        }
    }
    free(this->data);
    this->data = data;
}

std::string GameField::getName() {
    return name;
}

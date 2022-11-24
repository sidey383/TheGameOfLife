#include "GameField.h"
#include <utility>
#include <cstring>

using namespace gol;

GameField::GameField(GameRules& rules, std::string name, const bool *data, int width, int height):
        rules(rules), name(std::move(name)), width(width), height(height) {
    logger.debug() << "create game field " << this->name;
    this->data = new bool[width * height];
    memcpy(this->data, data, sizeof(bool) * width * height);
}

GameField::GameField() {
    this->rules = GameRules();
    this->width = 0;
    this->height = 0;
    this->data = nullptr;
    this->name = "";
}

GameField::GameField(GameField const &field) {
    this->rules = field.getRules();
    this->width = field.width;
    this->height = field.height;
    this->name = field.name;
    this->data = new bool[field.width*field.height];
    memcpy(this->data, field.data, field.width*field.height);
}

GameField::~GameField() {
    delete[] this->data;
}

unsigned int GameField::getArrayPose(int x, int y, int width, int height) {
    return (x >= 0 ?
            (x % width) :
            width + x%width)
           + width *
             (y >= 0 ?
              (y % height) :
              (height + (y % height)));
}

unsigned int GameField::getArrayPose(int x, int y) const {
    return getArrayPose(x, y, width, height);
}

bool GameField::getData(int x, int y) const {
    if(width <= 0 || height <= 0)
        return false;
    return data[getArrayPose(x, y)];
}

int GameField::getHeight() const {
    return height;
}

int GameField::getWidth() const {
    return width;
}

GameRules GameField::getRules() const {
    return rules;
}

void GameField::tick() {
    if(width <= 0 || height <= 0)
        return;
    logger.debug() << "tick field " << name;
    bool* newData = new bool[width * height];
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
            newData[getArrayPose(x, y)] = rules.isActive(this->data[getArrayPose(x, y)], sum);
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
    delete[] this->data;
    this->data = newData;
}

std::string GameField::getName() const {
    return name;
}

GameField& GameField::operator=(GameField const & field) {
    if(std::addressof(field) == this)
        return *this;
    this->width = field.width;
    this->height = field.height;
    this->rules = field.rules;
    this->data = new bool[width * height];
    memcpy(this->data, field.data, sizeof(bool) * width * height);
    return *this;
}

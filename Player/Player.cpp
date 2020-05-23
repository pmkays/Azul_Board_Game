#include "headers/Player.h"

Player::Player(std::string name, unsigned int dimensions){
    this->points = 0;
    this->mosaicStorage = std::make_shared<MosaicStorage>(dimensions);
    this->name = name;
}

Player::~Player(){
}

std::shared_ptr<MosaicStorage> Player::getMosaicStorage(){
    return this->mosaicStorage;
}

int Player::getPoints(){
    return this->points;
}

void Player::setPoints(unsigned const int points){
    this->points = points;
}

std::string Player::getName(){
    return this->name;
}


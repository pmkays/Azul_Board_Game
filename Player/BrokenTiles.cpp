#include "headers/BrokenTiles.h"
#include <iostream>

BrokenTiles::BrokenTiles(unsigned int dimensions) {
    if(dimensions == 5){
        this->maxSize = 7;
    } else {
        this->maxSize = 8;
    }
    this->size = 0;

    for(int i = 0; i < maxSize; i++){
        tiles[i] = nullptr;
    }
}

BrokenTiles::~BrokenTiles() {
    clear();
}

std::shared_ptr<Tile>* BrokenTiles::getTiles() {
    return tiles;
}

int BrokenTiles::calculatePointsLost() {
    unsigned int lostPoints = 0;
    unsigned const int numBrokenTiles = this->size;
    if (numBrokenTiles == 1) {
        lostPoints = 1;
    } else if (numBrokenTiles == 2) {
        lostPoints = 2;
    } else if (numBrokenTiles == 3) {
        lostPoints = 4;
    } else if (numBrokenTiles == 4) {
        lostPoints = 6;
    } else if (numBrokenTiles == 5) {
        lostPoints = 8;
    } else if (numBrokenTiles == 6) {
        lostPoints = 11;
    } else if (numBrokenTiles == 7) {
        lostPoints = 14;
    } else if (numBrokenTiles == 8){
        lostPoints = 18;
    }
    return lostPoints;
}

void BrokenTiles::clear() {
    for(int i = 0; i < size; ++i)
        this->tiles[i] = nullptr;
    
    this->size = 0;
}

void BrokenTiles::addTile(std::shared_ptr<Tile> tile) {
    tiles[size] = tile;
    ++size;
}

std::string BrokenTiles::toString() {
    std::string string = "Broken: ";
    for(int i = 0; i < maxSize; i++){
        if(tiles[i] == nullptr){
            Tile tile(Type::NONE);
            string += tile.getAnsiDisplay();
        } else{                 
            string += tiles[i]->getAnsiDisplay();
        }
    }
    return string;
}

int BrokenTiles::getSize() {
    return this->size;
}

std::string BrokenTiles::toSave() {
    std::string string;
    for(int i = 0; i < this->size; i++){
        string += tiles[i]->getColourType();
        string += " ";
    }

    return string;
}

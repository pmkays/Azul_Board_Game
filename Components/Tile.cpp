#include "headers/Tile.h"

Tile::Tile(const Type type) {
    this->type = type;
}

Tile::~Tile(){};

Tile::Tile(const Tile& other){
    this->type = other.type;
}

Type Tile::getType() const{
    return type; 
}

char Tile::getColourType() const{
    char tileChar = 'X';
    if(type == Type::RED)
        tileChar = 'R';
    if(type == Type::YELLOW)
        tileChar = 'Y';
    if(type == Type::DARK_BLUE)
        tileChar = 'B';
    if(type == Type::LIGHT_BLUE)
        tileChar = 'L';
    if(type == Type::BLACK)
        tileChar = 'U';
    if(type == Type::FIRST_PLAYER)
        tileChar = 'F';
    if(type == Type::ORANGE)
        tileChar = 'O';
    return tileChar;
}


    


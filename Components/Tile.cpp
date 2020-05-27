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

//this version is called straightaway when we want dots to be of a specific colour as per mosaic guidelines
std::string Tile::getAnsiDisplay(char colour, char symbol) const{
    std::string toReturn = ""; 
    if(colour == 'R'){
        toReturn += "\u001b[41;1m";
    }else if (colour == 'Y'){
        toReturn += "\u001b[43;1m";
    }else if(colour == 'B'){
        toReturn += "\u001b[44;1m";
    }else if (colour == 'L'){
        toReturn += "\x1b[48;5;39;1m";
    }else if (colour == 'U'){
        toReturn += "\u001b[40;1m";
    } else if (colour == 'O'){
        toReturn += "\x1b[48;5;202;1m";
    } else if (colour == 'X'){
        toReturn += "\x1b[48;5;240;1m";
        symbol = '.';
    }

    toReturn += " ";
    toReturn += symbol;
    toReturn += " \u001b[0m";

    return toReturn;
}

//this version is called by regular tiles with assigned colours
std::string Tile::getAnsiDisplay() const{
    char colour = getColourType();
    return getAnsiDisplay(colour, colour);
}



    


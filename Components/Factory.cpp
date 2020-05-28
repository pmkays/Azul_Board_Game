#include "headers/Factory.h"

Factory::Factory(){
    this->hasFirstPlayerToken = false;
}

Factory::~Factory(){}

void Factory::clear(){
    tiles.clear();
}

void Factory::addTile(const std::shared_ptr<Tile> tile){
    if(tile->getType() == Type::FIRST_PLAYER){
        this->hasFirstPlayerToken = true;
    }
    tiles.push_back(tile);
}

std::vector<std::shared_ptr<Tile>> Factory::getAllTiles() const{
    return this->tiles;
}

std::vector<std::shared_ptr<Tile>> Factory::getCopiedTilesAndRemove(){
    std::vector<std::shared_ptr<Tile>> copiedTiles; 

    for(unsigned int i = 0; i < tiles.size(); ++i){
        if(tiles[i] != nullptr){
            copiedTiles.push_back(tiles[i]);
        }
    }
    tiles.clear(); 
    return copiedTiles;
}

std::string Factory::toSave() const{
    std::string output;
    for(unsigned int i = 0; i < tiles.size(); ++i){
        output += tiles[i]->getColourType();
        output += " ";
    }
    return output;
}

std::string Factory::displayColouredTiles() const{
    std::string output;
    for(unsigned int i = 0; i < tiles.size(); ++i){
        output += tiles[i]->getAnsiDisplay();
    }
    return output;
}
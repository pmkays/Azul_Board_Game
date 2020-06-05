#include "headers/MosaicStorage.h"
#include <iostream>

MosaicStorage::MosaicStorage(unsigned int dimensions) {

    // Creating the grid as a 2d array of pointers
    this->grid = new std::shared_ptr<Tile>*[dimensions];
    for (unsigned int row = 0; row < dimensions; ++row) {
        this->grid[row] = new std::shared_ptr<Tile>[row+1];
    }

    this->mosaic = new Mosaic(dimensions);
    this->brokenTiles = new BrokenTiles(dimensions);
    this->dimensions = dimensions;
}

MosaicStorage::~MosaicStorage() {
    delete brokenTiles;
    brokenTiles = nullptr;


    for (unsigned int row = 0; row < dimensions; ++row) {
        clearRow(row);
    }
    delete[] grid;
    grid = nullptr;
}

std::shared_ptr<Tile>** MosaicStorage::getGrid() {
    return grid;
}

void MosaicStorage::resetDiscardTilesVector(){
    this->discardedTiles.clear();
};

void MosaicStorage::clearRow(unsigned const int row) {
    unsigned const int max = row+1;
    for (unsigned int col = 0; col < max; ++col) {
        grid[row][col] = nullptr;
    }
}

std::shared_ptr<Tile>* MosaicStorage::getRow(unsigned const int row) {
    return grid[row];
}

void MosaicStorage::addTileToDiscardedTiles(std::shared_ptr<Tile> tile){
    this->discardedTiles.push_back(tile);
}

bool MosaicStorage::isRowFull(unsigned const int row) {
    unsigned const int maxSizeOfRow = row+1;
    unsigned int i = 0;
    bool isFull = false;
    for (unsigned int counter = 0; counter < maxSizeOfRow; ++counter) {
        if (grid[row][i] != nullptr) {
            ++i;
        }
    }
    if (i==maxSizeOfRow) {
        isFull = true;
    }
    return isFull;
}

bool MosaicStorage::rowIsIncomplete(unsigned const int row) {
    bool hasSpace = false;
    for(unsigned int column = 0; column <= row; column++){
        if(grid[row][column] == nullptr){
            hasSpace = true;
        }
    }
    return hasSpace;
}

bool MosaicStorage::isValidAdd(Type type, unsigned const int row) {
    bool valid = false;
    int column = mosaic->getColourColumn(row, type);
    Type rowType = getRowType(row);
    if(type == rowType || rowType == Type::NONE){
        if (!isRowFull(row) && mosaic->isSpaceFree(row, column)) {
            valid = true;
        }
    }
    return valid;
}

//difference between normal one vs grey one is we can't check for the validity of the column yet, only row.
bool MosaicStorage::isValidAddForGrey(Type type, unsigned const int row) {
    bool valid = false;
    Type rowType = getRowType(row);
    
    if(type == rowType || rowType == Type::NONE){
        if (!isRowFull(row) && mosaic->hasFreeSpace(row) && !mosaic->alreadyExistsInRow(row, type)) {
            valid = true;
        }
    }   
    return valid;
}
void MosaicStorage::movePlayerTilesToMosaicManually(unsigned int row, unsigned int column){
    if(isRowFull(row)){
        std::shared_ptr<Tile>* tiles = getRow(row);
        for(unsigned int i = 0; i < (row + 1); ++i){
            if(i == 0){
                this->mosaic->addTile(tiles[i], row, column);
                grid[row][i] = nullptr;
            }
            else{
                this->discardedTiles.push_back(tiles[i]);
                grid[row][i] = nullptr;
            }
        }
    }
}

void MosaicStorage::moveTilesFromStorageRowToBroken(unsigned int row){
    if(isRowFull(row)){
        std::shared_ptr<Tile>* tiles = getRow(row);
        for(unsigned int i = 0; i < row + 1 ; ++i){
            if(tiles[i] != nullptr){
                this->brokenTiles->addTile(tiles[i]);
                grid[row][i] = nullptr;
            }
        }
    }
}

//we move the broken tiles to discarded tiles so we can later move them to the box lid
void MosaicStorage::endOfRoundDiscardBrokenTiles(){
    BrokenTiles* brokenTiles = getBrokenTiles();
    std::shared_ptr<Tile>* tiles = brokenTiles->getTiles();
    int size = brokenTiles->getSize();
    for(int i = 0; i < size; ++i){
        if(tiles[i]->getType() == Type::FIRST_PLAYER){
            tiles[i] = nullptr;
        }else{
            discardedTiles.push_back(tiles[i]);
            tiles[i] = nullptr;
        }
    }

    brokenTiles->clear();
}

//moves the tiles to their corresponding mosaic spaces in the mosaics while discarding the rest
void MosaicStorage::endOfRoundMove(){
    for(unsigned int row = 0; row< dimensions; ++row){
        if(isRowFull(row)){
            std::shared_ptr<Tile>* tiles = getRow(row);
            for(unsigned int i = 0; i < (row + 1); ++i){
                if(i == 0){
                    this->moveToMosaic(tiles[i],row);
                    grid[row][i] = nullptr;
                }
                else{
                    this->discardedTiles.push_back(tiles[i]);
                    grid[row][i] = nullptr;
                }
            }
        }
    }
}

void MosaicStorage::addTile(std::shared_ptr<Tile> tile, unsigned const int row) {
    if(!isRowFull(row)){
        unsigned int col = 0;
        while(grid[row][col] != nullptr) {
            ++col;
        }
        grid[row][col] = tile;
    }else{
        if(brokenTiles->getSize() < 7 ){
            brokenTiles->addTile(tile);
        }else{
            discardedTiles.push_back(tile);
        }
    }
}

Type MosaicStorage::getRowType(unsigned const int row){
    Type type = Type::NONE;
    for(unsigned int i = 0; i< row + 1; ++i){
        if(this->grid[row][i] != nullptr){
            type = grid[row][i]->getType();
        }
    }

    return type;

}

BrokenTiles* MosaicStorage::getBrokenTiles() {
    return brokenTiles;
}

void MosaicStorage::moveToMosaic(std::shared_ptr<Tile> tile, unsigned const int row) {
    int column = mosaic->getColourColumn(row, tile->getType());
    mosaic->addTile(tile, row, column);
}


void MosaicStorage::moveToDiscardedTiles(std::shared_ptr<Tile>* tiles) {
    unsigned int i = 0;
    while (tiles[i] != nullptr) {
        discardedTiles.push_back(tiles[i]);
        i++;
    }

}

void MosaicStorage::moveToBrokenTiles(std::shared_ptr<Tile>* tiles) {
    unsigned int i = 0;
    while (tiles[i] != nullptr) {
        brokenTiles->addTile(tiles[i]);
        ++i;
    }
}

Mosaic* MosaicStorage::getMosaic() {
    return mosaic;
}

std::vector<std::shared_ptr<Tile>>* MosaicStorage::getDiscardedTiles(){
    return &discardedTiles;
}

std::string MosaicStorage::rowToString(int index) const{
    std::string string = std::to_string(index+1) + ": ";

    for(unsigned int j = 0; j<dimensions-(index+1);j++){
            string+= "   ";
    }
    for(int i = index; i>=0; --i){
        if(grid[index][i] != nullptr){
            string += grid[index][i]->getAnsiDisplay();
        }else{
            Tile tile(Type::NONE);
            string += tile.getAnsiDisplay();
        }
    }
    return string;
}

std::string MosaicStorage::rowToSave(const int index) const {
    std::string string;
    for(int i = 0; i<= index; i++){    
        if(grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += ". ";
        }     
    }
    return string;
}

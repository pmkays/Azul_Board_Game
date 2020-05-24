#include "headers/Mosaic.h"

#include <iostream>

Mosaic::Mosaic(unsigned int dimensions){
    // Making 5x5 2d array for tiles
    this->grid = new std::shared_ptr<Tile>*[dimensions];
    for (unsigned int row = 0; row < dimensions; ++row) {
        grid[row] = new std::shared_ptr<Tile>[dimensions];
    }

    this->noOfBlacks = 0;
    this->noOfYellows = 0;
    this->noOfDarkBlues = 0;
    this->noOfLightBlues = 0;
    this->noOfReds = 0;
    this->noOfOranges = 0;
    this->pointsThisRound = 0;
    this->dimensions = dimensions;

}

Mosaic::~Mosaic(){
    // delete tiles
    for (unsigned int row = 0; row < dimensions; ++row) {
        for (unsigned int col = 0; col < dimensions; ++col) {
            if(grid[row][col] != nullptr){
                grid[row][col] = nullptr;
            }
        }
    }

    this->grid = nullptr;
    this->pointsThisRound = 0;
}

std::shared_ptr<Tile>** Mosaic::getGrid(){
    return this->grid;
}

bool Mosaic::findFullRow() const {
    bool fullRowFound = false;
    unsigned int counter = 0;

    for (unsigned int row = 0; row < dimensions; ++row) {
        for (unsigned int col = 0; col < dimensions; ++col) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == dimensions){
                    fullRowFound = true;
                }
            }
        }
        counter = 0;
    }

    return fullRowFound;
}

bool Mosaic::findFullCol(unsigned const int col) const {
    bool colIsFull = false;
    unsigned int counter = 0;

    for (unsigned int row = 0; row < dimensions; ++row) {
        if(grid[row][col] != nullptr){
            ++counter;
            if(counter == dimensions){
                colIsFull = true;
            }
        }
    }

    return colIsFull;
}

bool Mosaic::hasFreeSpace(unsigned const int row){
    bool free = false;
    for(unsigned int column = 0; column < dimensions; column++){
        if(grid[row][column] == nullptr){
            free = true;
        }
    }
    return free;
}

bool Mosaic::alreadyExistsInRow(unsigned const int row, Type type){
    bool exists = false;
    for(unsigned int column = 0; column < dimensions; column++){
        if(grid[row][column] != nullptr){
            if(grid[row][column]->getType() == type){
                exists = true;
            }
        }
    }
    return exists;
}

bool Mosaic::alreadyExistsInColumn(unsigned const int column, Type type){
    bool exists = false;
    for(unsigned int row = 0; row < dimensions; row++){
        if(grid[row][column] != nullptr){
            if(grid[row][column]->getType() == type){
                exists = true;
            }
        }
    }
    return exists;
}


unsigned int Mosaic::noOfFiveColours() const {
    unsigned int timesGotAll5Colours = 0;

    if(this->noOfBlacks == dimensions)
        ++timesGotAll5Colours;
    if(this->noOfReds == dimensions)
        ++timesGotAll5Colours;
    if(this->noOfDarkBlues == dimensions)
        ++timesGotAll5Colours;
    if(this->noOfLightBlues == dimensions)
        ++timesGotAll5Colours;
    if(this->noOfYellows == dimensions)
        ++timesGotAll5Colours;
    if(this->noOfOranges == dimensions)
        ++timesGotAll5Colours;
    return timesGotAll5Colours;
}

void Mosaic::numberOfSequentialTiles(unsigned int row, unsigned int col){
    int numSequential = 0;
    if (checkSequentialCols(row, col)) {
        ++numSequential;
    }
    if (checkSequentialRows(row,col)) {
        ++numSequential;
    }

    if (numSequential == 2) {
        pointsThisRound += 2;
    } else {
        pointsThisRound += 1;
    }
}

bool Mosaic::checkSequentialRows(unsigned const int row, unsigned const int col){
    bool hasSequential = false;
    unsigned int numberOfSequentialTiles = 0;
    bool sequential = true;

    // Check all values before the row
    for (int i = row-1; i >= 0; --i) {
        if (sequential && grid[i][col] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    sequential =  true;

    int convertedDimensions = dimensions;
    // Check all values after the row
    for (int i = row+1; i < convertedDimensions; ++i) {
        if (sequential && grid[i][col] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    pointsThisRound += numberOfSequentialTiles;

    return hasSequential;

}

bool Mosaic::checkSequentialCols(unsigned const int row, unsigned const int col){
    bool hasSequential = false;
    unsigned int numberOfSequentialTiles = 0;
    bool sequential = true;
    // Check all values before the row
    for (int i = col-1; i >= 0; --i) {
        if (sequential && grid[row][i] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    sequential = true;

     int convertedDimensions = dimensions;
    // Check all values after the row
    for (int i = col+1; i < convertedDimensions; ++i) { 
        if (sequential && grid[row][i] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    pointsThisRound += numberOfSequentialTiles;

    return hasSequential;
}

int Mosaic::getPointsThisRound(){
    return this->pointsThisRound;
}

void Mosaic::resetPoints(){
    this->pointsThisRound = 0;
}

bool Mosaic::isSpaceFree(unsigned int row, unsigned int col){
    bool valid = false;
    if(this->grid[row][col] == nullptr){
        valid = true;
    }
    return valid;
}

bool Mosaic::addTile(std::shared_ptr<Tile> tile, unsigned int row, unsigned int col){
    bool added = false;
    if(col >= 0 && col < dimensions && row >=0 && row < dimensions){
        if(grid[row][col] == nullptr){
            this->grid[row][col] = tile;
            numberOfSequentialTiles(row,col);
            incrementColorCounter(tile->getType());
            added = true;
        }
    }
    std::cout<<"Added a tile"<<std::endl;
    return added;
}

void Mosaic::incrementColorCounter(Type tileType){
     if(tileType == Type::RED)
        this->noOfReds++;
    if(tileType == Type::BLACK)
        this->noOfBlacks++;
    if(tileType == Type::DARK_BLUE)
        this->noOfDarkBlues++;
    if(tileType == Type::LIGHT_BLUE)
        this->noOfLightBlues++;
    if(tileType == Type::YELLOW)
        this->noOfYellows++;
    if(tileType == Type::ORANGE)
        this->noOfOranges++;
}

int Mosaic::getColourColumn(unsigned const int row, unsigned const int colour) const {
    int toReturn = -1;
    if (row >= 0 && row < dimensions && colour >= 0 && colour < dimensions) {
        if(dimensions == 5){
            toReturn = colourColumns[row][colour];
        } else if (dimensions == 6){
            toReturn = extraColourColumns[row][colour];
        }
    } 
    return toReturn;
}

std::string Mosaic::rowToString(int index) const{
    std::string string = "|| ";
    int convertedDimensions = dimensions;
    for(int i = 0; i< convertedDimensions; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += ". ";
        }
       
    }

    return string;
}

std::string Mosaic::templateRowToString(int index) const {
    std::string string = "";
    int dimensionsConverted = dimensions;
    for(int i = 0; i< dimensionsConverted; ++i){
        if(dimensions == 5){
            string += colourGrid[index][i];
        } else if (dimensions == 6)
        {
            string += extraColourGrid[index][i];
        }
        string += " ";
    }
    return string;
}

std::string Mosaic::rowToSave(int index) const {
    std::string string;
    int convertedDimensions = dimensions;
    for(int i = 0; i< convertedDimensions; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += toSavecolourGrid[index][i];
            string += " ";
        }      
    }
    return string;
}

std::string Mosaic::rowToSaveGrey(int index) const {
    std::string string;
    int convertedDimensions = dimensions;
    for(int i = 0; i< convertedDimensions; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += ".";
            string += " ";
        }      
    }
    return string;
}


int Mosaic::calculateEndGamePoints() {
    unsigned int endGamePoints = 0;


    endGamePoints += 2*numFullRows() + 7*numFullCols() + 10*noOfFiveColours();

    return endGamePoints;
}

unsigned int Mosaic::numFullCols() const {
    unsigned int counter = 0;
    unsigned int numCols = 0;

    for (unsigned int col = 0; col < dimensions; ++col) {
        for (unsigned int row = 0; row < dimensions; ++row) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == dimensions){
                    ++numCols;
                }
            }
        }
        counter = 0;
    }

    return numCols;
}

unsigned int Mosaic::numFullRows() const {
    unsigned int counter = 0;
    unsigned int numRows = 0;

    for (unsigned int row = 0; row < dimensions; ++row) {
        for (unsigned int col = 0; col < dimensions; ++col) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == dimensions){
                    ++numRows;
                }
            }
        }
        counter = 0;
    }

    return numRows;
}
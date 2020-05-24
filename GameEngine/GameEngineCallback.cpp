#include "GameEngineCallback.h"

GameEngineCallback::GameEngineCallback() {

}

GameEngineCallback::~GameEngineCallback() {

}

void GameEngineCallback::setDimensions(unsigned int dimensions){
    this->dimensions = dimensions;
}

void GameEngineCallback::playerBoardUpdate(Player* player) const {
    std::string outputString;
    std::shared_ptr<MosaicStorage> mStorage = player->getMosaicStorage();
    Mosaic* mosaic = player->getMosaicStorage()->getMosaic();

    outputString = "Mosaic for " + player->getName() + "\n"
        + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t\t " + mosaic->templateRowToString(0) + "\n";
        if(dimensions == 6){
            outputString += mStorage->rowToString(1) + mosaic->rowToString(1) + "  TEMPLATE " + mosaic->templateRowToString(1) + "\n";
            outputString += mStorage->rowToString(2) + mosaic->rowToString(2) + "  -------> " + mosaic->templateRowToString(2) + "\n";
        } else{
            outputString += mStorage->rowToString(1) + mosaic->rowToString(1) + "  TEMPLATE\t " + mosaic->templateRowToString(1) + "\n";
            outputString += mStorage->rowToString(2) + mosaic->rowToString(2) + "  ------->\t " + mosaic->templateRowToString(2) + "\n";
        }

        outputString += mStorage->rowToString(3) + mosaic->rowToString(3) + "\t\t " + mosaic->templateRowToString(3) + "\n"
        + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t\t " + mosaic->templateRowToString(4) + "\n";

        if(dimensions == 6)
        {
            outputString += mStorage->rowToString(5) + mosaic->rowToString(5) + "\t\t " + mosaic->templateRowToString(5) + "\n";
        }
    
    outputString += player->getMosaicStorage()->getBrokenTiles()->toString() + "\n";

    std::cout << outputString << std::endl;

}

void GameEngineCallback::playerTurnUpdate(const std::string playerName) const {
    std::cout<< "It is now " << playerName <<"'s turn. " << std::endl;
}

void GameEngineCallback::playerTurnResult(const std::string turnResult) const {
    std::cout << turnResult << std::endl;
}

void GameEngineCallback::playerEndOfRoundResult(Player* playerOne, Player* playerTwo) const {
    std::cout << playerOne->getName() << " Points: " << playerOne->getPoints() << std::endl;
    std::cout << playerTwo->getName() << " Points: " << playerTwo->getPoints() << std::endl;
    std::cout<< "\n=====End of round - Beginning next round!=====\n" << std::endl;
}

void GameEngineCallback::playerEndOfGameResult(Player* playerOne, Player* playerTwo) const {
    std::cout << playerOne->getName() << " Points: " << playerOne->getPoints() << std::endl;
    std::cout << playerTwo->getName() << " Points: " << playerTwo->getPoints() << std::endl;
    std::cout<< "\n=================End of Game=================" << std::endl;
    if (playerOne->getPoints() > playerTwo->getPoints()) {
        std::cout << "Winner is " << playerOne->getName() << "!\n" << std::endl;
    } else {
        std::cout << "Winner is " << playerTwo->getName() << "!\n" << std::endl;
    }
}

void GameEngineCallback::boardComponentUpdate(Factory** factory) const {
    std::cout<< "--------------------Next Turn------------------\n" << std::endl;
    std::vector<std::shared_ptr<Tile>> tilesOfCenter = factory[0]->getAllTiles();
    std::string outputString = "";

    outputString += "Factory 0: ";
    int size = tilesOfCenter.size();
    for(int j = 0; j < size; ++j){
        outputString.push_back(tilesOfCenter[j]->getColourType());
        outputString += " ";
    }
    outputString += "\n";

    for(int i = 1; i < 6; i++){
        std::vector<std::shared_ptr<Tile>> tiles = factory[i]->getAllTiles();
        outputString += "Factory " + std::to_string(i) += ": ";
        int size = tiles.size();
        for(int j = 0; j < size; ++j){
            outputString.push_back(tiles[j]->getColourType());
            outputString += " ";
        }
        outputString += "\n";
    }

    std::cout << outputString << std::endl;
}

void GameEngineCallback::promptColumnPlacement(unsigned int row, Player* player){
    std::cout <<player->getName() << ": For row " << row << ", please select what column you would like to place your tile."<<std::endl; 
}
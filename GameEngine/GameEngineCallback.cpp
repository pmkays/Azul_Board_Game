#include "GameEngineCallback.h"

GameEngineCallback::GameEngineCallback() {

}

GameEngineCallback::~GameEngineCallback() {

}

void GameEngineCallback::setDimensions(unsigned int dimensions){
    this->dimensions = dimensions;
}

void GameEngineCallback::setModeSelection(int modeSelection){
    this->modeSelection = modeSelection;
}

void GameEngineCallback::playerBoardUpdate(Player* player) const {
    std::string outputString;
    std::shared_ptr<MosaicStorage> mStorage = player->getMosaicStorage();
    Mosaic* mosaic = player->getMosaicStorage()->getMosaic();

    for(int i = 0; i < dimensions; i++){
        outputString += mStorage->rowToString(i) + mosaic->rowToString(i, modeSelection) + "\n";
    }
    
    outputString += player->getMosaicStorage()->getBrokenTiles()->toString() + "\n";

    // outputString = "Mosaic for " + player->getName() + "\n"
    //     + mStorage->rowToString(0) + mosaic->rowToString(0, modeSelection) + "\t\t " + mosaic->templateRowToString(0) + "\n";
    //     if(dimensions == 6){
    //         outputString += mStorage->rowToString(1) + mosaic->rowToString(1, modeSelection) + "  TEMPLATE " + mosaic->templateRowToString(1) + "\n";
    //         outputString += mStorage->rowToString(2) + mosaic->rowToString(2, modeSelection) + "  -------> " + mosaic->templateRowToString(2) + "\n";
    //     } else{
    //         outputString += mStorage->rowToString(1) + mosaic->rowToString(1, modeSelection) + "  TEMPLATE\t " + mosaic->templateRowToString(1) + "\n";
    //         outputString += mStorage->rowToString(2) + mosaic->rowToString(2, modeSelection) + "  ------->\t " + mosaic->templateRowToString(2) + "\n";
    //     }

    //     outputString += mStorage->rowToString(3) + mosaic->rowToString(3, modeSelection) + "\t\t " + mosaic->templateRowToString(3) + "\n"
    //     + mStorage->rowToString(4) + mosaic->rowToString(4, modeSelection) + "\t\t " + mosaic->templateRowToString(4) + "\n";

    //     if(dimensions == 6)
    //     {
    //         outputString += mStorage->rowToString(5) + mosaic->rowToString(5, modeSelection) + "\t\t " + mosaic->templateRowToString(5) + "\n";
    //     }
    
    // outputString += player->getMosaicStorage()->getBrokenTiles()->toString() + "\n";

    std::cout << outputString << std::endl;

}

void GameEngineCallback::playerBoardUpdateAfterTurn(Player** players, int numberOfPlayers) const{
    std::string outputString;
    for(int i = 0; i < numberOfPlayers; i+=2){
        outputString += "Mosaic for: " + players[i]->getName() + "\t\t\t\t";
        if(i+1 < numberOfPlayers){
            outputString += "Mosaic for: " + players[i+1]->getName() + "\n";
        }else{
            outputString +="\n";
        }
        for(int j = 0; j < dimensions; j++){
            outputString += players[i]->getMosaicStorage()->rowToString(j) + players[i]->getMosaicStorage()->getMosaic()->rowToString(j, modeSelection) + "\t";
            if(i+1 < numberOfPlayers){
                outputString += players[i+1]->getMosaicStorage()->rowToString(j) + players[i+1]->getMosaicStorage()->getMosaic()->rowToString(j, modeSelection) + "\n";
            }else{
                outputString += "\n";
            }
        }
        outputString += players[i]->getMosaicStorage()->getBrokenTiles()->toString() + "\t\t";
        if(i+1 < numberOfPlayers){
            outputString += players[i+1]->getMosaicStorage()->getBrokenTiles()->toString() + "\n\n";
        }else{
            outputString += "\n\n";
        }
    }
    std::cout << outputString <<std::endl;
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

void GameEngineCallback::playerEndOfRoundResult(Player* player) const {
    std::cout << player->getName() << " Points: " << player->getPoints() << std::endl;
}

void GameEngineCallback::endOfRoundStatement() const{
    std::cout<< "\n=====End of round - Beginning next round!=====\n" << std::endl;
}

// void GameEngineCallback::playerEndOfGameResult(Player* playerOne, Player* playerTwo) const {
//     std::cout << playerOne->getName() << " Points: " << playerOne->getPoints() << std::endl;
//     std::cout << playerTwo->getName() << " Points: " << playerTwo->getPoints() << std::endl;
//     std::cout<< "\n=================End of Game=================" << std::endl;
//     if (playerOne->getPoints() > playerTwo->getPoints()) {
//         std::cout << "Winner is " << playerOne->getName() << "!\n" << std::endl;
//     } else {
//         std::cout << "Winner is " << playerTwo->getName() << "!\n" << std::endl;
//     }
// }

void GameEngineCallback::playerEndOfGameResult(Player** players, int numberOfPlayers) const {
    std::cout<< "\n=================End of Game=================" << std::endl;
    int max = 0;
    std::string winner = ""; 
    for(int i = 0; i < numberOfPlayers; i++){
        if(players[i]->getPoints() > max){
            max = players[i]->getPoints();
            winner = players[i]->getName();
        }
    }
    std::cout << "Winner: " << winner << "!\n" << std::endl;
}

void GameEngineCallback::boardComponentUpdate(Factory** factory, int numberOfFactories, int numberOfCentralFactories) const {
    std::cout<< "--------------------Next Turn------------------\n" << std::endl;

    std::string outputString = "";

    for(int i  =  0; i < numberOfCentralFactories; i++){
        outputString += "[Central] Factory " + std::to_string(i) += ": " +factory[i]->displayColouredTiles() + "\n";
    }
    outputString += "\n";
    for(int i = numberOfCentralFactories; i < numberOfFactories; i+=2){
        outputString += "Factory " + std::to_string(i) += ": " +factory[i]->displayColouredTiles() + "\t\t";
        if(i + 1 < numberOfFactories){
            outputString += "Factory " + std::to_string(i+1) += ": " +factory[i+1]->displayColouredTiles() + "\n";
        }else{
            outputString+="\n";
        }
    }
    std::cout << outputString << std::endl;
}

void GameEngineCallback::promptColumnPlacement(unsigned int row, Player* player){
    std::cout <<player->getName() << ": For row " << row << ", please select what column you would like to place your tile."<<std::endl; 
}
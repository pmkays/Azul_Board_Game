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

    outputString += "Mosaic for: " + player->getName() + "\n";
    for(int i = 0; i < dimensions; i++){
        outputString += mStorage->rowToString(i) + mosaic->rowToString(i, modeSelection) + "\n";
    }
    
    outputString += player->getMosaicStorage()->getBrokenTiles()->toString() + "\n";

    std::cout << outputString << std::endl;

}

void GameEngineCallback::playerBoardUpdateAfterTurn(Player** players, int numberOfPlayers) const{
    std::string outputString;
    for(int i = 0; i < numberOfPlayers; i+=2){

        //print mosaic titles; we must do checks while printing if there is an odd number of players
        outputString += "Mosaic for: " + players[i]->getName() + "\t\t\t\t";
        if(modeSelection == Mode::ORANGE_BOARD){
            outputString += "\t";
        }

        if(i+1 < numberOfPlayers){
            outputString += "Mosaic for: " + players[i+1]->getName() + "\n";
        }else{
            outputString +="\n";
        }

        //print the storage row and the mosaics
        for(int j = 0; j < dimensions; j++){
            outputString += players[i]->getMosaicStorage()->rowToString(j) + players[i]->getMosaicStorage()->getMosaic()->rowToString(j, modeSelection) + "\t";
            if(i+1 < numberOfPlayers){
                outputString += players[i+1]->getMosaicStorage()->rowToString(j) + players[i+1]->getMosaicStorage()->getMosaic()->rowToString(j, modeSelection) + "\n";
            }else{
                outputString += "\n";
            }
        }

        //print the broken tiles
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
        outputString += factoryHelper(factory[i], i, "\t\t");
        if(i + 1 < numberOfFactories){
            outputString += factoryHelper(factory[i+1], i+1, "\n");
        }else{
            outputString+="\n";
        }
    }
    std::cout << outputString << std::endl;
}

std::string GameEngineCallback::factoryHelper(Factory* factory, int index, std::string formatter) const{
    std::string outputString ="";
    outputString +=  "Factory " + std::to_string(index) + ": ";
    if(factory->getAllTiles().size() != 0){
        outputString += factory->displayColouredTiles() + formatter;
    }else{
        for(int i = 0; i < dimensions - 1; i++){
            outputString += "   ";
        }
        outputString += formatter;
    }
    return outputString;
}

void GameEngineCallback::promptColumnPlacement(unsigned int row, Player* player){
    std::cout <<player->getName() << ": For row " << row << ", please select what column you would like to place your tile."<<std::endl; 
}

void GameEngineCallback::gameplayHelp(int numberOfCentralFactories){
    if (modeSelection == Mode::GREY){
        greyBoardHelp();
    } else{
        if(numberOfCentralFactories == 1){
            generalTurnHelp();
            helpFooter(); 
        }else{
            twoCFHelp();
        }
    }
}

void GameEngineCallback::greyBoardHelp(){
    generalTurnHelp(); 
    std::cout << "Once the round has finished, you will be prompted to select which mosaic column you would like to move a tile to from a full storage row. \n"
              << "Tip: Be careful and make sure you don't run into a dead end!\n"<< std::endl;
    helpFooter();
}


void GameEngineCallback::twoCFHelp(){
     std::cout << "To take turns, type the following command in this structure: 'turn {factory number} {tile} {storage row} {central factory number}'.\n"
              << "For example: 'turn 5 U 2 1' means: take the U tiles from factory 5, place them into storage row 2, and place the remaining tiles in central factory 1.\n"
              << "Alternatively, you can choose to place tiles in your broken row voluntarily by typing 'B' instead of a storage row number\n" << std::endl;
    helpFooter();
}
void GameEngineCallback::generalTurnHelp(){
    std::cout << "To take turns, type the following command in this structure: 'turn {factory number} {tile} {storage row}'.\n"
              << "For example: 'turn 5 U 2' means: take the U tiles from factory 5 and place them into storage row 2.\n"
              << "Alternatively, you can choose to place tiles in your broken row voluntarily by typing 'B' instead of a storage row number\n" << std::endl;
}
void GameEngineCallback::helpFooter(){
    std::cout << "If you would like to save the game, type the following command in this structure: 'save {file name}'. \n"
             << "For more game-specific rules and how-to-play guides, please visit: https://www.ultraboardgames.com/azul/game-rules.php\n\n" << std::endl;

}
